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
        m_pRenderCollector = new RenderCollector{};

        DirectX::XMStoreFloat4x4( &m_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &m_pRenderCollector->ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

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
        SAFE_DELETE( m_pRenderCollector );
    }

    void WireframeRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &m_pRenderCollector->ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &m_pRenderCollector->ViewInverseMatrix, cameraTransformMat );
    }

    void WireframeRenderer::BeginScene( const EditorCamera &editorCamera )
    {
        m_pRenderCollector->ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

        DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &viewMatrix );
        DirectX::XMStoreFloat4x4(
            &m_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );
    }

    void WireframeRenderer::EndScene()
    {
        ClearDrawList();
    }

    void WireframeRenderer::OnRender()
    {
        GraphicsContext *pContext = RenderEngine::GetContext();

        pContext->BindRasterizerState( s_pWireframeRasterizerState );

        for ( const DrawCommand &drawCommand : m_pRenderCollector->DrawList )
        {
            pContext->BindVertexBuffer( drawCommand.pVertexBuffer );
            pContext->BindIndexBuffer( drawCommand.pIndexBuffer );
            pContext->BindShader( drawCommand.pShader );

            drawCommand.pShader->UploadMat4( "ViewProjection", m_pRenderCollector->ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            // drawCommand.pShader->UploadMat4( "ViewInverse", s_pRenderCollector->ViewInverseMatrix );

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->Count, drawCommand.pShader );
        }

        pContext->UnbindRasterizerState();
    }

    void WireframeRenderer::Submit( const scene::BoxColliderComponent &boxColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DirectX::XMMATRIX finalTransformMat = DirectX::XMLoadFloat4x4( &worldTransform );
        DirectX::XMVECTOR translationVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR scaleVec{};

        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &translationVec, finalTransformMat );
        DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3( &boxColliderComponent.Offset );
        auto finalTranslationVec = DirectX::XMVectorAdd( translationVec, offsetVec );
        DirectX::XMVECTOR sizeVec = DirectX::XMLoadFloat3( &boxColliderComponent.Size );

        sizeVec = DirectX::XMVectorDivide( sizeVec, DirectX::XMVECTOR{ 2, 2, 2 } );

        auto finalScaleVec = DirectX::XMVectorMultiply( scaleVec, sizeVec );
        finalTransformMat = DirectX::XMMatrixScalingFromVector( finalScaleVec ) *
                            DirectX::XMMatrixRotationQuaternion( rotationVec ) *
                            DirectX::XMMatrixTranslationFromVector( finalTranslationVec );

        DirectX::XMFLOAT4X4 finalTransform{};
        DirectX::XMStoreFloat4x4( &finalTransform, finalTransformMat );

        DrawCommand drawCommand{ boxColliderComponent.pWireframeMesh->GetVertexBuffer(),
            boxColliderComponent.pWireframeMesh->GetIndexBuffer(),
            RenderEngine::GetShaderLibrary().Get( "PosCol" ),
            finalTransform };
        m_pRenderCollector->DrawList.emplace_back( drawCommand );
    }

    void WireframeRenderer::ClearDrawList()
    {
        m_pRenderCollector->DrawList.clear();
    }
}