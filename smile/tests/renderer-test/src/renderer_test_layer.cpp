/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        renderer_test_layer.cpp
 * @author      Zenn Geeraerts
 * @created     10 March 2026
 * @brief       Renderer test application layer
 */
#include "renderer_test_layer.h"

#include "smile/core/application/application.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/renderable.h"

namespace smile::graphic
{
    void RendererTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();

        m_RenderEngine = RenderEngine::Create( rhi::RendererBackendType::D3D11 );
        m_SwapChain = m_RenderEngine->CreateSwapChain( &window );
        m_Renderer = m_RenderEngine->CreateRenderer();

        auto &resourceManager = m_RenderEngine->GetResourceManager();
        auto &materialSystem = m_RenderEngine->GetMaterialSystem();
        const auto &shaderLibrary = m_RenderEngine->GetShaderLibrary();

        DirectX::XMFLOAT4X4 viewMatrix{};
        {
            DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f ) *
                                             DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f ) *
                                             DirectX::XMMatrixTranslation( 0.0f, 2.0f, -10.0f );

            DirectX::XMStoreFloat4x4( &viewMatrix, DirectX::XMMatrixInverse( nullptr, transformMat ) );
        }

        DirectX::XMFLOAT4X4 projectionMatrix{};
        {
            auto projectionMatrixMat =
                DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( 30.f ), 1.778f, 0.1f, 2500.f );

            DirectX::XMStoreFloat4x4( &projectionMatrix, projectionMatrixMat );
        }

        m_Scene = m_RenderEngine->CreateScene( m_SwapChain );
        m_Scene->GetView().SetViewProjectionMatrix( viewMatrix, projectionMatrix );

        {
            MaterialLayout layout{};
            layout.Parameters.EmplaceBack( "Color", MaterialParameterType::Float3, 0u, 12u );
            layout.Parameters.EmplaceBack( "UseTexture", MaterialParameterType::Int, 12u, 4u );

            layout.Textures.EmplaceBack( "Diffuse", 0u );

            layout.CbSlot = 2;
            layout.CbSize = 16u;
            layout.Visibility = { rhi::ShaderStage::Pixel };

            auto vertexShader = shaderLibrary.Get( "pos_tex.vs" );
            auto pixelShader = shaderLibrary.Get( "col_tex.ps" );

            auto program = Program::Create( vertexShader, pixelShader );

            MaterialDescriptor desc{};
            desc.ShaderProgram = program;
            desc.Parameters["Color"] = DirectX::XMFLOAT3{ 1.0f, 0.0f, 0.0f };
            desc.Parameters["UseTexture"] = 0;
            desc.TextureBindings["Diffuse"] = {};

            m_Material = materialSystem.CreateMaterial( "Mat", layout, desc );
        }

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

            m_VertexBuffer = resourceManager.CreateVertexBuffer( quadVertices,
                quadVerticesCount,
                { { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORD" } } );

            const Count quadIndexCount = 6;
            Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };

            m_IndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndexCount );
        }

        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, DirectX::XMMatrixIdentity() );

        Renderable &renderable = m_Scene->AddRenderable( SceneLayer::World );
        renderable.SetWorldTransform( worldTransform );
        RenderPrimitive &prim = renderable.AddPrimitive();
        prim.SetGeometry( m_VertexBuffer, m_IndexBuffer, rhi::PrimitiveTopology::TriangleList );
        prim.SetMaterialInstance( m_Material.GetDefaultInstance() );
    }

    void RendererTestLayer::OnDetach()
    {
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        m_Material.SetParameter( "Color", DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f } );

        m_Scene->GetView().OnUpdate();

        m_Renderer->BeginFrame();
        m_Renderer->OnRender( *m_Scene );
        m_Renderer->EndFrame( *m_SwapChain );
    }

    void RendererTestLayer::OnEvent( window::Event &event )
    {
    }

    void RendererTestLayer::OnImGuiRender()
    {
    }
}