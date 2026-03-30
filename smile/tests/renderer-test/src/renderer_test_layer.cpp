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
#include "smile/graphic/renderer/debug_renderer.h"
#include "smile/graphic/renderer/forward_renderer.h"
#include "smile/graphic/renderer/material/material_system.h"

namespace smile::graphic
{
    void RendererTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        RenderEngine::Initialize( &window );

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

        m_View.SetViewProjectionMatrix( viewMatrix, projectionMatrix );

        {
            MaterialLayout layout{};
            layout.Parameters.EmplaceBack( "Color", MaterialParameterType::Float3, 0u, 12u );
            layout.Parameters.EmplaceBack( "UseTexture", MaterialParameterType::Int, 12u, 4u );

            layout.Textures.EmplaceBack( "Diffuse", 0u );

            layout.CbSlot = 2;
            layout.CbSize = 16u;
            layout.Visibility = { rhi::ShaderStage::Pixel };

            auto &shaderLibrary = RenderEngine::GetShaderLibrary();
            auto vertexShader = shaderLibrary.GetShader( "pos_tex.vs" );
            auto pixelShader = shaderLibrary.GetShader( "col_tex.ps" );

            auto program = Program::Create( vertexShader, pixelShader );

            MaterialDescriptor desc{};
            desc.ShaderProgram = program;
            desc.Parameters["Color"] = DirectX::XMFLOAT3{ 1.0f, 0.0f, 0.0f };
            desc.Parameters["UseTexture"] = 0;
            desc.TextureBindings["Diffuse"] = nullptr;

            m_Material = RenderEngine::GetMaterialSystem().CreateMaterial( layout, desc );
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

            auto &resourceManager = RenderEngine::GetRenderContext().GetResourceManager();

            m_VertexBuffer = resourceManager.CreateVertexBuffer( quadVertices,
                quadVerticesCount,
                { { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORD" } } );

            const Count quadIndexCount = 6;
            Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };

            m_IndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndexCount );
        }
    }

    void RendererTestLayer::OnDetach()
    {
        RenderEngine::ShutDown();
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderContext = RenderEngine::GetRenderContext();
        auto &debugRenderer = DebugRenderer::GetInstance();
        auto &forwardRenderer = ForwardRenderer::GetInstance();

        m_Material->SetParameter( "Color", DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f } );
        RenderEngine::GetMaterialSystem().Update();

        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, DirectX::XMMatrixIdentity() );

        rhi::RenderState renderState{};

        forwardRenderer.Submit( { m_VertexBuffer, m_IndexBuffer, m_Material, worldTransform, renderState } );

        m_View.OnUpdate();

        renderContext.Clear(
            renderContext.GetBackBuffer(), math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        renderContext.BeginFrame();

        debugRenderer.BeginScene( m_View );
        debugRenderer.OnRender( renderContext.GetBackBuffer() );
        debugRenderer.EndScene();

        forwardRenderer.BeginScene( m_View );
        forwardRenderer.OnRender( renderContext.GetBackBuffer() );
        forwardRenderer.EndScene();

        renderContext.EndFrame();
    }

    void RendererTestLayer::OnEvent( window::Event &event )
    {
    }

    void RendererTestLayer::OnImGuiRender()
    {
    }
}