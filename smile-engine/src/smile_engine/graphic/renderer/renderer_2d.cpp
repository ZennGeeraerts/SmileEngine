/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile_engine/graphic/render_command.h"

namespace smile::graphic
{
    struct Renderer2DStorage final
    {
        Ref< VertexBuffer > pQuadVertexBuffer;
        Ref< IndexBuffer > pQuadIndexBuffer;
        Ref< Shader > pShader;

        DirectX::XMFLOAT4X4 ViewProjectionMatrix;
    };

    static Renderer2DStorage *s_pStorage;

    void Renderer2D::Initialize()
    {
        s_pStorage = new Renderer2DStorage{};

        const Uint32 quadVerticesCount = 12;
        float quadVertices[] = { -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

        VertexBufferDescriptor vertexBufferDesc{};
        vertexBufferDesc.pVertices = quadVertices;
        vertexBufferDesc.Count = quadVerticesCount;
        vertexBufferDesc.Stride = 12;

        s_pStorage->pQuadVertexBuffer = RenderEngine::GetDevice()->CreateVertexBuffer( vertexBufferDesc );

        const Uint32 quadIndicesCount = 6;
        Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };

        IndexBufferDescriptor indexBufferDesc{};
        indexBufferDesc.pIndices = quadIndices;
        indexBufferDesc.Count = quadIndicesCount;

        s_pStorage->pQuadIndexBuffer = RenderEngine::GetDevice()->CreateIndexBuffer( indexBufferDesc );

        s_pStorage->pShader = RenderEngine::GetShaderLibrary().Get( "PosCol" );
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
    }

    void Renderer2D::BeginScene( const EditorCamera &editorCamera )
    {
        s_pStorage->ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();
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

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT4 &color )
    {
        GraphicsContext *pContext = RenderEngine::GetContext();

        pContext->BindPrimitiveTopology( PrimitiveTopology::TriangleList );

        pContext->BindVertexBuffer( s_pStorage->pQuadVertexBuffer );
        pContext->BindIndexBuffer( s_pStorage->pQuadIndexBuffer );
        pContext->BindShader( s_pStorage->pShader );

        s_pStorage->pShader->UploadMat4( "ViewProjection", s_pStorage->ViewProjectionMatrix );
        s_pStorage->pShader->UploadMat4( "World", worldTransform );
        s_pStorage->pShader->UploadFloat3( "color", DirectX::XMFLOAT3{ color.x, color.y, color.z } );

        RenderCommand::DrawIndexed( s_pStorage->pQuadIndexBuffer->Count, s_pStorage->pShader );

        pContext->UnbindPrimitiveTopology();
    }
}