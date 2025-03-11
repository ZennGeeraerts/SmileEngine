/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"

namespace smile::graphic
{
    struct Renderer2DStorage final
    {
        memory::Ref< VertexBuffer > pQuadVertexBuffer;
        memory::Ref< IndexBuffer > pQuadIndexBuffer;
        memory::Ref< Shader > pShader;

        DirectX::XMFLOAT4X4 ViewProjectionMatrix;
    };

    static Renderer2DStorage *s_pStorage;

    void Renderer2D::Initialize()
    {
        s_pStorage = new Renderer2DStorage{};

        const Uint32 quadVerticesCount = 12;
        float quadVertices[] = { -0.5f,
            -0.5f,
            0.0f,
            -1,
            -1,
            /*1*/ -0.5f,
            0.5f,
            0.0f,
            -1,
            1,
            /*2*/ 0.5f,
            -0.5f,
            0.0f,
            1,
            -1 /*3*/,
            0.5f,
            0.5f,
            0.0f,
            1,
            1 /*4*/ };

        GPUBufferDescriptor vertexBufferDesc{};
        vertexBufferDesc.pData = quadVertices;
        vertexBufferDesc.Size = quadVerticesCount * sizeof( float );
        vertexBufferDesc.BindFlags = BufferBindFlags::VertexBuffer;

        s_pStorage->pQuadVertexBuffer = RenderEngine::GetRenderSystem().GetResourceManager().CreateVertexBuffer(
            vertexBufferDesc, sizeof( float ) * 5 );

        const Uint32 quadIndicesCount = 6;
        Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };

        GPUBufferDescriptor indexBufferDesc{};
        indexBufferDesc.pData = quadIndices;
        indexBufferDesc.Size = quadIndicesCount * sizeof( Uint32 );
        indexBufferDesc.BindFlags = BufferBindFlags::IndexBuffer;

        s_pStorage->pQuadIndexBuffer =
            RenderEngine::GetRenderSystem().GetResourceManager().CreateIndexBuffer( indexBufferDesc, quadIndicesCount );

        s_pStorage->pShader = RenderEngine::GetShaderLibrary().Get( "PosColTex" );
    }

    void Renderer2D::ShutDown()
    {
        delete s_pStorage;
    }

    void Renderer2D::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &s_pStorage->ViewProjectionMatrix, viewProjectionMatrixMat );

        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();
        renderSystem.BindShader( s_pStorage->pShader );
        s_pStorage->pShader->UploadMat4( "ViewProjection", s_pStorage->ViewProjectionMatrix );
        renderSystem.UnbindShader();
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::OnRender()
    {
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        const DirectX::XMFLOAT4 &color )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixScaling( size.x, size.y, 1 ) *
                                              DirectX::XMMatrixRotationRollPitchYaw( 0, 0, rotation ) *
                                              DirectX::XMMatrixTranslation( position.x, position.y, 0 );
        DirectX::XMFLOAT4X4 worldTransform{};
        DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );

        DrawQuad( worldTransform, color );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
        const ecs::SpriteRendererComponent &spriteRendererComponent )
    {
        if ( spriteRendererComponent.pTexture )
            DrawQuad( worldTransform, spriteRendererComponent.pTexture, spriteRendererComponent.Color );
        else
            DrawQuad( worldTransform, spriteRendererComponent.Color );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT4 &color )
    {
        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.BindPrimitiveTopology( PrimitiveTopology::TriangleList );

        renderSystem.BindVertexBuffer( s_pStorage->pQuadVertexBuffer );
        renderSystem.BindIndexBuffer( s_pStorage->pQuadIndexBuffer );
        renderSystem.BindShader( s_pStorage->pShader );

        s_pStorage->pShader->UploadMat4( "World", worldTransform );
        s_pStorage->pShader->UploadFloat3( "Color", DirectX::XMFLOAT3{ color.x, color.y, color.z } );
        s_pStorage->pShader->UploadBool( "UseTexture", false );

        renderSystem.DrawIndexed( s_pStorage->pQuadIndexBuffer->Count );

        renderSystem.UnbindPrimitiveTopology();
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
        const memory::Ref< Texture > &pTexture,
        const DirectX::XMFLOAT4 &color )
    {
        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.BindPrimitiveTopology( PrimitiveTopology::TriangleList );

        renderSystem.BindVertexBuffer( s_pStorage->pQuadVertexBuffer );
        renderSystem.BindIndexBuffer( s_pStorage->pQuadIndexBuffer );
        renderSystem.BindShader( s_pStorage->pShader );

        s_pStorage->pShader->UploadMat4( "World", worldTransform );
        s_pStorage->pShader->UploadFloat3( "Color", DirectX::XMFLOAT3{ color.x, color.y, color.z } );
        s_pStorage->pShader->UploadBool( "UseTexture", true );
        s_pStorage->pShader->UploadTexture( "Diffuse", pTexture );

        renderSystem.DrawIndexed( s_pStorage->pQuadIndexBuffer->Count );

        renderSystem.UnbindPrimitiveTopology();
    }
}