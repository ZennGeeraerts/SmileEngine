/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/material/material_system.h"

namespace smile::graphic
{
    void ForwardRenderPass::Initialize()
    {
        auto &resourceManager = RenderEngine::GetRenderContext().GetResourceManager();
        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );
            cameraCBDesc.Add( "ViewInverse", ConstantType::Mat4 );

            m_pCameraCB = resourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            ConstantBufferDescriptor perObjectCBDesc{};
            perObjectCBDesc.Add( "World", ConstantType::Mat4 );

            m_PerObjectCB = resourceManager.CreateConstantBuffer( perObjectCBDesc );
        }

        {
            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, m_pCameraCB->GetHandle() ) },
                { rhi::BindingSetElement::CreateConstantBuffer( 1, m_PerObjectCB->GetHandle() ) } };

            m_pBindingSet = resourceManager.CreateBindingSet( bindingSetDesc, { rhi::ShaderStage::Vertex } );
        }
    }

    void ForwardRenderPass::SetupMaterial( MaterialInstance::Ref materialInstance,
        const rhi::RenderState &renderState,
        GraphicsState &graphicsState )
    {
        auto &materialSystem = RenderEngine::GetMaterialSystem();
        materialSystem.UpdateMaterialInstance( materialInstance );

        const PipelineKey key{ materialInstance, renderState };

        auto it = m_Pipelines.FindItemAtKey( key );
        if ( it == m_Pipelines.end() )
        {
            it = CreatePipeline( materialInstance, renderState );
        }

        graphicsState.pPipeline = it.GetItem();

        const auto &materialData = materialSystem.GetMaterialData( materialInstance );
        graphicsState.pBindings.PushBack( materialData.Bindings );
    }

    primitive::HashMap< ForwardRenderPass::PipelineKey, GraphicsPipeline::Ref >::Iterator
    ForwardRenderPass::CreatePipeline( MaterialInstance::ConstRef materialInstance,
        const rhi::RenderState &renderState )
    {
        const auto &materialData = RenderEngine::GetMaterialSystem().GetMaterialData( materialInstance );
        auto &resourceManager = RenderEngine::GetRenderContext().GetResourceManager();

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

        psoDesc.pVertexShader =
            resourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );

        psoDesc.pPixelShader = resourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        rhi::BindingLayout bindingLayout{ { rhi::ShaderStage::Vertex } };
        bindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
        bindingLayout.AddElement( { 1, rhi::ResourceType::ConstantBuffer } );

        psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );
        psoDesc.BindingLayouts.PushBack( materialData.Bindings->GetLayout() );

        psoDesc.RenderState = renderState;

        auto pipeline = resourceManager.CreateGraphicsPipeline( psoDesc );
        return m_Pipelines.Insert( PipelineKey{ materialInstance, renderState }, std::move( pipeline ) );
    }

    void ForwardRenderPass::ShutDown()
    {
        ClearDrawList();
    }

    void ForwardRenderPass::BeginPass( const View &view )
    {
        view.FillConstants( m_ViewConstants );

        m_pCameraCB->Update( &m_ViewConstants );
    }

    void ForwardRenderPass::Submit( const DrawItem &drawItem )
    {
        m_RenderCollector.DrawList.PushBack( drawItem );
    }

    void ForwardRenderPass::Submit( DrawItem &&drawItem )
    {
        m_RenderCollector.DrawList.PushBack( std::move( drawItem ) );
    }

    void ForwardRenderPass::Execute( Framebuffer::Ref framebuffer )
    {
        RenderContext &renderContext = RenderEngine::GetRenderContext();

        renderContext.FillConstantBuffer( m_pCameraCB );

        for ( const DrawItem &drawItem : m_RenderCollector.DrawList )
        {
            m_PerObjectCB->Update( &drawItem.WorldTransform );
            renderContext.FillConstantBuffer( m_PerObjectCB );

            GraphicsState state{};
            state.pFramebuffer = framebuffer;
            state.VertexBuffers.PushBack( { drawItem.pVertexBuffer, 0u, 0u } );
            state.IndexBuffer = IndexBufferBinding{ drawItem.pIndexBuffer, rhi::Format::R32_UINT, 0u };
            state.pBindings.PushBack( m_pBindingSet );

            SetupMaterial( drawItem.MaterialInstance, drawItem.RenderState, state );

            renderContext.SetGraphicsState( state );
            renderContext.DrawIndexed( drawItem.pIndexBuffer->GetIndexCount() );
        }
    }

    void ForwardRenderPass::EndPass()
    {
        ClearDrawList();
    }

    void ForwardRenderPass::ClearDrawList()
    {
        m_RenderCollector.DrawList.Clear();
    }
}