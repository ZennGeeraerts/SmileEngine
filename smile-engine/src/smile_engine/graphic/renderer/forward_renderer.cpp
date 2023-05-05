/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_renderer.h"

#include "smile_engine/graphic/render_engine.h"
#include "smile_engine/graphic/render_command.h"

namespace smile::graphic
{
    RenderCollector *ForwardRenderer::s_pRenderCollector = new RenderCollector{};

    void ForwardRenderer::Initialize()
    {
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewProjectionMatrix, DirectX::XMMatrixIdentity() );
    }

    void ForwardRenderer::ShutDown()
    {
        ClearDrawlist();
        SAFE_DELETE( s_pRenderCollector );
    }

    void ForwardRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewInverseMatrix, cameraTransformMat );
    }

    void ForwardRenderer::BeginScene( const EditorCamera &editorCamera )
    {
        s_pRenderCollector->ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

        DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &viewMatrix );
        DirectX::XMStoreFloat4x4(
            &s_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );
    }

    void ForwardRenderer::Submit( const Ref< VertexBuffer > &pVertexBuffer,
        const Ref< IndexBuffer > &pIndexBuffer,
        const Ref< Shader > &pShader,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        s_pRenderCollector->DrawList.emplace_back(
            DrawCommand{ pVertexBuffer, pIndexBuffer, pShader, worldTransform } );
    }

    void ForwardRenderer::Submit( const scene::MeshRendererComponent &meshRendererComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        Submit( meshRendererComponent.pVertexBuffer,
            meshRendererComponent.pIndexBuffer,
            meshRendererComponent.pShader,
            worldTransform );
    }

    void ForwardRenderer::Submit( const scene::StaticMeshComponent &staticMeshComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &mesh : staticMeshComponent.pMeshes )
        {
            Submit( mesh->GetVertexBuffer(),
                mesh->GetIndexBuffer(),
                staticMeshComponent.pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void ForwardRenderer::Submit( const scene::SkinnedMeshComponent &skinnedMeshComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &mesh : skinnedMeshComponent.pMeshes )
        {
            Submit( mesh->GetVertexBuffer(),
                mesh->GetIndexBuffer(),
                skinnedMeshComponent.pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void ForwardRenderer::OnRender()
    {
        GraphicsContext *pContext = RenderEngine::GetContext();

        for ( const DrawCommand &drawCommand : s_pRenderCollector->DrawList )
        {
            pContext->BindVertexBuffer( drawCommand.pVertexBuffer );
            pContext->BindIndexBuffer( drawCommand.pIndexBuffer );
            pContext->BindShader( drawCommand.pShader );

            drawCommand.pShader->UploadMat4( "ViewProjection", s_pRenderCollector->ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            drawCommand.pShader->UploadMat4( "ViewInverse", s_pRenderCollector->ViewInverseMatrix );

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->Count, drawCommand.pShader );
        }
    }

    void ForwardRenderer::EndScene()
    {
        ClearDrawlist();
    }

    void ForwardRenderer::ClearDrawlist()
    {
        s_pRenderCollector->DrawList.clear();
    }
}