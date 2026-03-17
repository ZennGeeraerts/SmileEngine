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

namespace smile::graphic
{
    void RendererTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        RenderEngine::Initialize( &window );

        m_Camera = { DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( 30.f ), 1.778f, 0.1f, 2500.f ) };

        DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f ) *
                                         DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f ) *
                                         DirectX::XMMatrixTranslation( 0.0f, 2.0f, -10.0f );
        DirectX::XMStoreFloat4x4( &m_CameraTransform, transformMat );
    }

    void RendererTestLayer::OnDetach()
    {
        RenderEngine::ShutDown();
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderSystem = RenderEngine::GetRenderSystem();
        auto &debugRenderer = DebugRenderer::GetInstance();

        renderSystem.Clear(
            renderSystem.GetBackBuffer(), math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        renderSystem.BeginFrame();

        debugRenderer.BeginScene( m_Camera, m_CameraTransform );
        debugRenderer.OnRender( renderSystem.GetBackBuffer() );
        debugRenderer.EndScene();

        renderSystem.EndFrame();
    }

    void RendererTestLayer::OnEvent( window::Event &event )
    {
    }

    void RendererTestLayer::OnImGuiRender()
    {
    }
}