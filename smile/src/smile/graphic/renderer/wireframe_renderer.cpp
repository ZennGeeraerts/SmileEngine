/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "wireframe_renderer.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"

namespace smile::graphic
{
    void WireframeRenderer::Initialize()
    {
        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

        ResourceManager &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
        RasterizerStateDescriptor rasterizerStateDesc{};
        rasterizerStateDesc.CullMode = CullMode::None;
        rasterizerStateDesc.FillMode = FillMode::WireFrame;
        rasterizerStateDesc.EnableDepthClip = true;

        s_pWireframeRasterizerState = resourceManager.CreateRasterizerState( rasterizerStateDesc );
    }

    void WireframeRenderer::ShutDown()
    {
        ClearDrawList();
    }

    void WireframeRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewInverseMatrix, cameraTransformMat );
    }

    void WireframeRenderer::EndScene()
    {
        ClearDrawList();
    }

    void WireframeRenderer::OnRender()
    {
        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.BindPrimitiveTopology( PrimitiveTopology::TriangleList );
        renderSystem.BindRasterizerState( s_pWireframeRasterizerState );

        if ( !m_RenderCollector.DrawList.empty() )
        {
            DrawCommand &drawCommand = m_RenderCollector.DrawList[0];
            renderSystem.BindShader( drawCommand.pShader );
            drawCommand.pShader->UploadMat4( "ViewProjection", m_RenderCollector.ViewProjectionMatrix );
        }

        for ( const DrawCommand &drawCommand : m_RenderCollector.DrawList )
        {
            renderSystem.BindVertexBuffer( drawCommand.pVertexBuffer );
            renderSystem.BindIndexBuffer( drawCommand.pIndexBuffer );

            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            drawCommand.pShader->UploadFloat3( "Color", DirectX::XMFLOAT3{ 1, 1, 1 } );

            renderSystem.DrawIndexed( drawCommand.pIndexBuffer->Count );
        }

        renderSystem.UnbindRasterizerState();
        renderSystem.UnbindPrimitiveTopology();
    }

    void WireframeRenderer::Submit( const physics::ecs::BoxColliderComponent &boxColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4( &worldTransform );
        DirectX::XMVECTOR translationVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR scaleVec{};

        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &translationVec, worldTransformMat );
        DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3( &boxColliderComponent.Box.Center );
        DirectX::XMVECTOR sizeVec = DirectX::XMLoadFloat3( &boxColliderComponent.Box.Size );

        sizeVec = DirectX::XMVectorDivide( sizeVec, DirectX::XMVECTOR{ 1, 1, 1 } );

        auto colliderTransformMat = DirectX::XMMatrixScalingFromVector( sizeVec ) * DirectX::XMMatrixIdentity() *
                                    DirectX::XMMatrixTranslationFromVector( offsetVec );

        DirectX::XMMATRIX finalTransformMat = colliderTransformMat * worldTransformMat;

        DirectX::XMFLOAT4X4 finalTransform{};
        DirectX::XMStoreFloat4x4( &finalTransform, finalTransformMat );

        DrawCommand drawCommand{ boxColliderComponent.pWireframeMesh->pVertexBuffer,
            boxColliderComponent.pWireframeMesh->pIndexBuffer,
            RenderEngine::GetShaderLibrary().Get( "PosCol" ),
            finalTransform };
        m_RenderCollector.DrawList.emplace_back( drawCommand );
    }

    void WireframeRenderer::Submit( const physics::ecs::SphereColliderComponent &sphereColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DrawCommand drawCommand{ sphereColliderComponent.pWireframeMesh->pVertexBuffer,
            sphereColliderComponent.pWireframeMesh->pIndexBuffer,
            RenderEngine::GetShaderLibrary().Get( "PosCol" ),
            worldTransform };
        m_RenderCollector.DrawList.emplace_back( drawCommand );
    }

    void WireframeRenderer::ClearDrawList()
    {
        m_RenderCollector.DrawList.clear();
    }
}