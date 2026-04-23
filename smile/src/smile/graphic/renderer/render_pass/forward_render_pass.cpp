/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/material/material_system.h"

namespace smile::graphic
{
    ForwardRenderPass::ForwardRenderPass( RenderContext &context,
        ResourceManager &resourceManager,
        MaterialSystem &materialSystem ) noexcept
        : m_Context{ context }, m_ResourceManager{ resourceManager }, m_MaterialSystem{ materialSystem }
    {
    }

    void ForwardRenderPass::Initialize()
    {
        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );
            cameraCBDesc.Add( "ViewInverse", ConstantType::Mat4 );

            m_CameraCB = m_ResourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            ConstantBufferDescriptor perObjectCBDesc{};
            perObjectCBDesc.Add( "World", ConstantType::Mat4 );

            m_PerObjectCB = m_ResourceManager.CreateConstantBuffer( perObjectCBDesc );
        }

        {
            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, m_CameraCB.GetHandle() ) },
                { rhi::BindingSetElement::CreateConstantBuffer( 1, m_PerObjectCB.GetHandle() ) } };

            m_BindingSet = m_ResourceManager.CreateBindingSet( bindingSetDesc, { rhi::ShaderStage::Vertex } );
        }
    }

    void ForwardRenderPass::SetupMaterial( MaterialInstance::Ref materialInstance,
        const rhi::RenderState &renderState,
        GraphicsState &graphicsState )
    {
        m_MaterialSystem.UpdateMaterialInstance( materialInstance );

        const PipelineKey key{ materialInstance, renderState };

        auto it = m_Pipelines.FindItemAtKey( key );
        if ( it == m_Pipelines.end() )
        {
            it = CreatePipeline( materialInstance, renderState );
        }

        graphicsState.Pipeline = it.GetItem();

        const auto &materialData = m_MaterialSystem.GetMaterialData( materialInstance );
        graphicsState.Bindings.PushBack( materialData.Bindings );
    }

    primitive::HashMap< ForwardRenderPass::PipelineKey, GraphicsPipeline >::Iterator ForwardRenderPass::CreatePipeline(
        MaterialInstance::ConstRef materialInstance,
        const rhi::RenderState &renderState )
    {
        const auto &materialData = m_MaterialSystem.GetMaterialData( materialInstance );

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

        psoDesc.VertexShader =
            m_ResourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );

        psoDesc.PixelShader = m_ResourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        rhi::BindingLayout bindingLayout{ { rhi::ShaderStage::Vertex } };
        bindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
        bindingLayout.AddElement( { 1, rhi::ResourceType::ConstantBuffer } );

        psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );
        psoDesc.BindingLayouts.PushBack( materialData.Bindings.GetLayout() );

        psoDesc.RenderState = renderState;

        auto pipeline = m_ResourceManager.CreateGraphicsPipeline( psoDesc );
        return m_Pipelines.Insert( PipelineKey{ materialInstance, renderState }, std::move( pipeline ) );
    }

    void ForwardRenderPass::ShutDown()
    {
        ClearDrawList();
    }

    void ForwardRenderPass::BeginPass( const View &view )
    {
        view.FillConstants( m_ViewConstants );

        m_CameraCB.Update( &m_ViewConstants );
    }

    void ForwardRenderPass::Submit( const DrawItem &drawItem )
    {
        m_RenderCollector.DrawList.PushBack( drawItem );
    }

    void ForwardRenderPass::Submit( DrawItem &&drawItem )
    {
        m_RenderCollector.DrawList.PushBack( std::move( drawItem ) );
    }

    void ForwardRenderPass::Execute( const Framebuffer &framebuffer )
    {
        m_Context.FillConstantBuffer( m_CameraCB );

        for ( const DrawItem &drawItem : m_RenderCollector.DrawList )
        {
            m_PerObjectCB.Update( &drawItem.WorldTransform );
            m_Context.FillConstantBuffer( m_PerObjectCB );

            GraphicsState state{};
            state.Framebuffer = framebuffer;
            state.VertexBuffers.PushBack( { drawItem.VertexBuffer, 0u, 0u } );
            state.IndexBuffer = IndexBufferBinding{ drawItem.IndexBuffer, rhi::Format::R32_UINT, 0u };
            state.Bindings.PushBack( m_BindingSet );

            SetupMaterial( drawItem.MaterialInstance, drawItem.RenderState, state );

            m_Context.SetGraphicsState( state );
            m_Context.DrawIndexed( drawItem.IndexBuffer.GetIndexCount() );
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