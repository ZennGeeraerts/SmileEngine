/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_renderer.h"

#include "engine/graphic/renderer/render_command.h"

namespace smile::graphic
{
    RenderCollector ForwardRenderer::s_RenderCollector{};

    void ForwardRenderer::Initialize()
    {
        DirectX::XMStoreFloat4x4( &s_RenderCollector.ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &s_RenderCollector.ViewProjectionMatrix, DirectX::XMMatrixIdentity() );
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
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &s_RenderCollector.ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &s_RenderCollector.ViewInverseMatrix, cameraTransformMat );
    }

    void ForwardRenderer::Submit( const memory::Ref< VertexBuffer > &pVertexBuffer,
        const memory::Ref< IndexBuffer > &pIndexBuffer,
        const memory::Ref< Shader > &pShader,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        s_RenderCollector.DrawList.emplace_back( DrawCommand{ pVertexBuffer, pIndexBuffer, pShader, worldTransform } );
    }

    void ForwardRenderer::Submit( const ecs::MeshRendererComponent &meshRendererComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        if ( !meshRendererComponent.pMesh || !meshRendererComponent.pMaterial )
            return;

        Submit( meshRendererComponent.pMesh->pVertexBuffer,
            meshRendererComponent.pMesh->pIndexBuffer,
            meshRendererComponent.pMaterial->GetShader(),
            worldTransform );
    }

    void ForwardRenderer::Submit( const ecs::SkinnedMeshRendererComponent &skinnedMeshRendererComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        if ( !skinnedMeshRendererComponent.pSkinnedMesh || !skinnedMeshRendererComponent.pMaterial )
            return;

        Submit( skinnedMeshRendererComponent.pSkinnedMesh->pVertexBuffer,
            skinnedMeshRendererComponent.pSkinnedMesh->pIndexBuffer,
            skinnedMeshRendererComponent.pMaterial->GetShader(),
            worldTransform );
    }

    void ForwardRenderer::OnRender()
    {
        GraphicsContext *pContext = RenderCommand::GetGraphicsContext();

        pContext->BindPrimitiveTopology( PrimitiveTopology::TriangleList );

        for ( const DrawCommand &drawCommand : s_RenderCollector.DrawList )
        {
            pContext->BindVertexBuffer( drawCommand.pVertexBuffer );
            pContext->BindIndexBuffer( drawCommand.pIndexBuffer );
            pContext->BindShader( drawCommand.pShader );

            drawCommand.pShader->UploadMat4( "ViewProjection", s_RenderCollector.ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            drawCommand.pShader->UploadMat4( "ViewInverse", s_RenderCollector.ViewInverseMatrix );

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->Count, drawCommand.pShader );
        }

        pContext->UnbindPrimitiveTopology();
    }

    void ForwardRenderer::EndScene()
    {
        ClearDrawList();
    }

    void ForwardRenderer::ClearDrawList()
    {
        s_RenderCollector.DrawList.clear();
    }
}