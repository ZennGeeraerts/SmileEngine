/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "wireframe_renderer.h"

#include "smile_engine/graphic/render_command.h"

namespace smile::graphic
{
    void WireframeRenderer::Initialize()
    {
        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &m_RenderCollector.ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

        GraphicsDevice *pDevice = RenderEngine::GetDevice();
        RasterizerStateDescriptor rasterizerStateDesc{};
        rasterizerStateDesc.CullMode = CullMode::None;
        rasterizerStateDesc.FillMode = FillMode::WireFrame;
        rasterizerStateDesc.EnableDepthClip = true;

        s_pWireframeRasterizerState = pDevice->CreateRasterizerState( rasterizerStateDesc );
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

    void WireframeRenderer::BeginScene( const EditorCamera &editorCamera )
    {
        m_RenderCollector.ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

        DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &viewMatrix );
        DirectX::XMStoreFloat4x4(
            &m_RenderCollector.ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );
    }

    void WireframeRenderer::EndScene()
    {
        ClearDrawList();
    }

    void WireframeRenderer::OnRender()
    {
        GraphicsContext *pContext = RenderEngine::GetContext();

        pContext->BindPrimitiveTopology( PrimitiveTopology::TriangleList );
        pContext->BindRasterizerState( s_pWireframeRasterizerState );

        for ( const DrawCommand &drawCommand : m_RenderCollector.DrawList )
        {
            pContext->BindVertexBuffer( drawCommand.pVertexBuffer );
            pContext->BindIndexBuffer( drawCommand.pIndexBuffer );
            pContext->BindShader( drawCommand.pShader );

            drawCommand.pShader->UploadMat4( "ViewProjection", m_RenderCollector.ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            drawCommand.pShader->UploadFloat3( "Color", DirectX::XMFLOAT3{ 1, 1, 1 } );

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->Count, drawCommand.pShader );
        }

        pContext->UnbindRasterizerState();
        pContext->UnbindPrimitiveTopology();
    }

    void WireframeRenderer::Submit( const scene::BoxColliderComponent &boxColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4( &worldTransform );
        DirectX::XMVECTOR translationVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR scaleVec{};

        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &translationVec, worldTransformMat );
        DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3( &boxColliderComponent.Offset );
        DirectX::XMVECTOR sizeVec = DirectX::XMLoadFloat3( &boxColliderComponent.Size );

        sizeVec = DirectX::XMVectorDivide( sizeVec, DirectX::XMVECTOR{ 2, 2, 2 } );

        auto colliderTransformMat = DirectX::XMMatrixScalingFromVector( sizeVec ) * 
                                    DirectX::XMMatrixIdentity() *
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

    void WireframeRenderer::ClearDrawList()
    {
        m_RenderCollector.DrawList.clear();
    }
}