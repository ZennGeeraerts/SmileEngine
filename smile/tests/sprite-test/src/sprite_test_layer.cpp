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
 * @file        sprite_test_layer.cpp
 * @author      Zenn Geeraerts
 * @created     31 March 2026
 * @brief       Sprite test application layer
 */
#include "sprite_test_layer.h"

#include "smile/core/application/application.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/sprite/renderer_2d.h"

namespace smile::graphic
{
    void SpriteTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        RenderEngine::Initialize( &window );
        Renderer2D::GetInstance().Initialize();

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
    }

    void SpriteTestLayer::OnDetach()
    {
        Renderer2D::GetInstance().ShutDown();
        RenderEngine::ShutDown();
    }

    void SpriteTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderContext = RenderEngine::GetRenderContext();
        auto &renderer2D = Renderer2D::GetInstance();
        
        //RenderEngine::GetMaterialSystem().Update();

        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, DirectX::XMMatrixIdentity() );

        renderer2D.DrawQuad( worldTransform, DirectX::XMFLOAT3{ 1.0f, 0.0f, 0.0f } );

        m_View.OnUpdate();

        renderContext.Clear(
            renderContext.GetBackBuffer(), math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        renderContext.BeginFrame();

        {
            renderer2D.BeginScene( m_View );
            renderer2D.OnRender( renderContext.GetBackBuffer() );
            renderer2D.EndScene();
        }

        renderContext.EndFrame();
    }

    void SpriteTestLayer::OnEvent( window::Event &event )
    {
    }

    void SpriteTestLayer::OnImGuiRender()
    {
    }
}