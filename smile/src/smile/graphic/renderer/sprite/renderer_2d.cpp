/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile/common/memory/scope.h"

#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/renderable.h"
#include "smile/graphic/shader/shader_library.h"

namespace smile::graphic
{
    struct Renderer2DStorage final
    {
        VertexBuffer QuadVertexBuffer;
        IndexBuffer QuadIndexBuffer;
        RenderScene *Scene{ nullptr };
    };

    static memory::Scope< Renderer2DStorage > s_pStorage;

    void Renderer2D::Initialize( ResourceManager &resourceManager, const ShaderLibrary &shaderLib )
    {
        s_pStorage = memory::CreateScope< Renderer2DStorage >();

        auto vertexShaderAsset = shaderLib.Get( "pos_tex.vs" );
        auto pixelShaderAsset = shaderLib.Get( "col_tex.ps" );

        auto program = Program::Create( vertexShaderAsset, pixelShaderAsset );
        {
            const Count quadVerticesCount = 12;
            float quadVertices[] = { -0.5f,
                -0.5f,
                0.0f,
                0.0f,
                1.0f,
                /*1*/ -0.5f,
                0.5f,
                0.0f,
                0.0f,
                0.0f,
                /*2*/ 0.5f,
                -0.5f,
                0.0f,
                1.0f,
                1.0f /*3*/,
                0.5f,
                0.5f,
                0.0f,
                1.0f,
                0.0f /*4*/ };

            s_pStorage->QuadVertexBuffer =
                resourceManager.CreateVertexBuffer( quadVertices, quadVerticesCount, program->GetVertexLayout() );
        }

        {
            const Count quadIndicesCount = 6;
            Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };
            s_pStorage->QuadIndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndicesCount );
        }
    }

    void Renderer2D::BeginFrame( RenderScene &scene )
    {
        SM_ASSERT( s_pStorage );
        s_pStorage->Scene = &scene;
    }

    void Renderer2D::ShutDown()
    {
        s_pStorage.Reset();
    }

    void Renderer2D::DrawSprite( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        MaterialInstance materialInstance )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixScaling( size.x, size.y, 1 ) *
                                              DirectX::XMMatrixRotationRollPitchYaw( 0, 0, rotation ) *
                                              DirectX::XMMatrixTranslation( position.x, position.y, 0 );
        DirectX::XMFLOAT4X4 worldTransform{};
        DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );

        DrawSprite( worldTransform, materialInstance );
    }

    void Renderer2D::DrawSprite( const DirectX::XMFLOAT4X4 &worldTransform, MaterialInstance materialInstance )
    {
        SM_ASSERT( s_pStorage && s_pStorage->Scene );
        Renderable &renderable = s_pStorage->Scene->AddRenderable( SceneLayer::World );
        renderable.SetWorldTransform( worldTransform );

        RenderPrimitive &primitive = renderable.AddPrimitive();
        primitive.SetGeometry(
            s_pStorage->QuadVertexBuffer, s_pStorage->QuadIndexBuffer, rhi::PrimitiveTopology::TriangleList );
        primitive.SetMaterialInstance( materialInstance );
    }
}