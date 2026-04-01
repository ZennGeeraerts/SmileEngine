/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/memory/scope.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/forward_renderer.h"
#include "smile/graphic/shader/shader_asset.h"

namespace smile::graphic
{
    /*struct DrawItem2D final
    {
        DrawItem2D( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT3 &color ) noexcept
            : WorldTransform{ worldTransform }, Color{ color }, UseTexture{ false }
        {
        }

        DrawItem2D( const DirectX::XMFLOAT4X4 &worldTransform, Texture::ConstRef pTexture ) noexcept
            : WorldTransform{ worldTransform }, UseTexture{ true }, pTexture{ pTexture }
        {
        }

        DirectX::XMFLOAT4X4 WorldTransform;
        DirectX::XMFLOAT3 Color{ 0.0f, 0.0f, 0.0f };
        bool UseTexture;
        Texture::ConstRef pTexture = nullptr;
    };*/

    struct Renderer2DStorage final
    {
        VertexBuffer::Ref pQuadVertexBuffer;
        IndexBuffer::Ref pQuadIndexBuffer;

        Material::Ref pColorMaterial = nullptr;
        Material::Ref pTextureMaterial = nullptr;
        Sampler::Ref pSampler;
    };

    static memory::Scope< Renderer2DStorage > s_pStorage;

    void Renderer2D::Initialize()
    {
        s_pStorage = memory::CreateScope< Renderer2DStorage >();

        auto &resourceManager = RenderEngine::GetRenderContext().GetResourceManager();
        const auto &shaderLibrary = RenderEngine::GetShaderLibrary();

        auto vertexShaderAsset = shaderLibrary.GetShader( "pos_tex.vs" );
        auto pixelShaderAsset = shaderLibrary.GetShader( "col_tex.ps" );

        auto program = Program::Create( vertexShaderAsset, pixelShaderAsset );
        {
            const Count quadVerticesCount = 12;
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

            s_pStorage->pQuadVertexBuffer =
                resourceManager.CreateVertexBuffer( quadVertices, quadVerticesCount, program->GetVertexLayout() );
        }

        {
            const Count quadIndicesCount = 6;
            Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };
            s_pStorage->pQuadIndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndicesCount );
        }
    }

    void Renderer2D::ShutDown()
    {
        s_pStorage.Reset();
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        const DirectX::XMFLOAT3 &color )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixScaling( size.x, size.y, 1 ) *
                                              DirectX::XMMatrixRotationRollPitchYaw( 0, 0, rotation ) *
                                              DirectX::XMMatrixTranslation( position.x, position.y, 0 );
        DirectX::XMFLOAT4X4 worldTransform{};
        DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );

        DrawQuad( worldTransform, color );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT3 &color )
    {
        ForwardRenderer::GetInstance().Submit( DrawItem{ s_pStorage->pQuadVertexBuffer,
            s_pStorage->pQuadIndexBuffer,
            s_pStorage->pColorMaterial,
            worldTransform,
            rhi::RenderState{} } );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, Texture::ConstRef pTexture )
    {
        ForwardRenderer::GetInstance().Submit( DrawItem{ s_pStorage->pQuadVertexBuffer,
            s_pStorage->pQuadIndexBuffer,
            s_pStorage->pTextureMaterial,
            worldTransform,
            rhi::RenderState{} } );
    }
}