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

#include "smile/graphic/renderer/renderer.h"
#include "smile/graphic/renderer/debug_render_pass.h"

namespace smile::graphic
{
    void RendererTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        Renderer::Initialize( &window );
        Renderer::GetRenderPassList().PushBack( memory::CreateRef< DebugRenderPass >() );

        m_Camera = { DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( 30.f ), 1.778f, 0.1f, 2500.f ) };

        DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f ) *
                                         DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f ) *
                                         DirectX::XMMatrixTranslation( 0.0f, 2.0f, -10.0f );
        DirectX::XMStoreFloat4x4( &m_CameraTransform, transformMat );
    }

    void RendererTestLayer::OnDetach()
    {
        Renderer::ShutDown();
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        Renderer::OnRender( m_Camera, m_CameraTransform );
    }

    void RendererTestLayer::OnEvent( window::Event &event )
    {
    }

    void RendererTestLayer::OnImGuiRender()
    {
    }
}