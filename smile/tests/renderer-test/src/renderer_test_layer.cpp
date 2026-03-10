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
    }

    void RendererTestLayer::OnDetach()
    {
        RenderEngine::ShutDown();
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderSystem = RenderEngine::GetRenderSystem();
        auto &debugRenderer = DebugRenderer::GetInstance();

        renderSystem.BeginFrame();

        DirectX::XMFLOAT4X4 cameraTransform;
        DirectX::XMStoreFloat4x4( &cameraTransform, DirectX::XMMatrixIdentity() );

        debugRenderer.BeginScene( m_Camera, cameraTransform );
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