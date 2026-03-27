/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_renderer.h"

#include "render_engine.h"
#include "material/material_system.h"

namespace smile::graphic
{
    void ForwardRenderer::Initialize()
    {
        DirectX::XMStoreFloat4x4( &m_RenderCollector.View.ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &m_RenderCollector.View.ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
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

    void ForwardRenderer::SetupMaterial( Material::ConstRef material,
        const rhi::RenderState &renderState,
        GraphicsState &graphicsState )
    {
        const PipelineKey key{ material, renderState };

        auto it = m_Pipelines.FindItemAtKey( key );
        if ( it == m_Pipelines.end() )
        {
            it = CreatePipeline( material, renderState );
        }

        graphicsState.pPipeline = it.GetItem();

        const auto &materialData = RenderEngine::GetMaterialSystem().GetMaterialData( material );
        graphicsState.pBindings.PushBack( materialData.Bindings );
    }

    primitive::HashMap< ForwardRenderer::PipelineKey, GraphicsPipeline::Ref >::Iterator
    ForwardRenderer::CreatePipeline( Material::ConstRef material, const rhi::RenderState &renderState )
    {
        const auto &materialData = RenderEngine::GetMaterialSystem().GetMaterialData( material );
        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

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
        return m_Pipelines.Insert( PipelineKey{ material, renderState }, std::move( pipeline ) );
    }

    void ForwardRenderer::ShutDown()
    {
        ClearDrawList();
    }

    void ForwardRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = DirectX::XMMatrixTranspose( viewMatrixMat * projectionMatrixMat );

        DirectX::XMStoreFloat4x4( &m_RenderCollector.View.ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &m_RenderCollector.View.ViewInverseMatrix, cameraTransformMat );

        m_pCameraCB->Update( &m_RenderCollector.View );
    }

    void ForwardRenderer::Submit( const DrawCommand &drawItem )
    {
        m_RenderCollector.DrawList.PushBack( drawItem );
    }

    void ForwardRenderer::Submit( DrawCommand &&drawItem )
    {
        m_RenderCollector.DrawList.PushBack( std::move( drawItem ) );
    }

    void ForwardRenderer::OnRender( Framebuffer::Ref framebuffer )
    {
        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.FillConstantBuffer( m_pCameraCB );

        for ( const DrawCommand &drawCommand : m_RenderCollector.DrawList )
        {
            m_PerObjectCB->Update( &drawCommand.WorldTransform );
            renderSystem.FillConstantBuffer( m_PerObjectCB );

            GraphicsState state{};
            state.pFramebuffer = framebuffer;
            state.VertexBuffers.PushBack( { drawCommand.pVertexBuffer, 0u, 0u } );
            state.IndexBuffer = IndexBufferBinding{ drawCommand.pIndexBuffer, rhi::Format::R32_UINT, 0u };
            state.pBindings.PushBack( m_pBindingSet );

            SetupMaterial( drawCommand.Material, drawCommand.RenderState, state );

            renderSystem.SetGraphicsState( state );
            renderSystem.DrawIndexed( drawCommand.pIndexBuffer->GetIndexCount() );
        }
    }

    void ForwardRenderer::EndScene()
    {
        ClearDrawList();
    }

    void ForwardRenderer::ClearDrawList()
    {
        m_RenderCollector.DrawList.Clear();
    }
}