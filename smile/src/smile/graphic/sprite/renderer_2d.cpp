/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile/common/memory/scope.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/forward_renderer.h"

namespace smile::graphic
{
    struct Renderer2DStorage final
    {
        VertexBuffer::Ref pQuadVertexBuffer;
        IndexBuffer::Ref pQuadIndexBuffer;
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

    void Renderer2D::DrawSprite( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        Material::ConstRef material )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixScaling( size.x, size.y, 1 ) *
                                              DirectX::XMMatrixRotationRollPitchYaw( 0, 0, rotation ) *
                                              DirectX::XMMatrixTranslation( position.x, position.y, 0 );
        DirectX::XMFLOAT4X4 worldTransform{};
        DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );

        DrawSprite( worldTransform, material );
    }

    void Renderer2D::DrawSprite( const DirectX::XMFLOAT4X4 &worldTransform, Material::ConstRef material )
    {
        ForwardRenderer::GetInstance().Submit( DrawItem{ s_pStorage->pQuadVertexBuffer,
            s_pStorage->pQuadIndexBuffer,
            material,
            worldTransform,
            rhi::RenderState{} } );
    }
}