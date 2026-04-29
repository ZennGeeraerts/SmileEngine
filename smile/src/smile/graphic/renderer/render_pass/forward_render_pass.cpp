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

            m_ResourceManager.CreateBindingSetAndLayout(
                bindingSetDesc, { rhi::ShaderStage::Vertex }, m_BindingLayout, m_BindingSet );
        }
    }

    void ForwardRenderPass::SetupMaterial( MaterialInstance::Ref materialInstance, GraphicsState &graphicsState )
    {
        m_MaterialSystem.UpdateMaterialInstance( materialInstance );

        auto it = m_Pipelines.FindItemAtKey( materialInstance );
        if ( it == m_Pipelines.end() )
        {
            it = CreatePipeline( materialInstance );
        }

        graphicsState.Pipeline = it.GetItem();

        const auto &materialData = m_MaterialSystem.GetMaterialData( materialInstance );
        graphicsState.Bindings.PushBack( materialData.Bindings );
    }

    primitive::HashMap< MaterialInstance::ConstRef, GraphicsPipeline >::Iterator ForwardRenderPass::CreatePipeline(
        MaterialInstance::ConstRef materialInstance )
    {
        const auto &materialData = m_MaterialSystem.GetMaterialData( materialInstance );

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

        psoDesc.VertexShader =
            m_ResourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );

        psoDesc.PixelShader = m_ResourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        psoDesc.BindingLayouts.PushBack( m_BindingLayout );
        psoDesc.BindingLayouts.PushBack( materialData.BindingLayout );

        psoDesc.RenderState = materialInstance->GetMaterial().GetLayout().RenderState;

        auto pipeline = m_ResourceManager.CreateGraphicsPipeline( psoDesc );
        return m_Pipelines.Insert( materialInstance, std::move( pipeline ) );
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

            SetupMaterial( drawItem.MaterialInstance, state );

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