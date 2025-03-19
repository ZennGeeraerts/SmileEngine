/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/skybox_renderer.h"
#include "smile/graphic/camera/ecs/camera_component.h"

#include "smile/core/window/window.h"
// #include "smile/core/world/ecs/transform_component.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    Scene::Scene( const window::Window *pWindow )
    {
        FramebufferDescriptor frameBufferDesc{};
        frameBufferDesc.Attachments = { { FramebufferTextureFormat::RGBA8, true },
            FramebufferTextureFormat::Depth,
            { FramebufferTextureFormat::RGBA8, true } };
        frameBufferDesc.Width = pWindow->GetWidth();
        frameBufferDesc.Height = pWindow->GetHeight();
        frameBufferDesc.IsSwapChainTarget = false;

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
        m_pFramebuffer = resourceManager.CreateFramebuffer( frameBufferDesc );
        m_pFramebuffer->ClearColor = { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] };
    }

    void Scene::OnRender()
    {
        auto &renderSystem = RenderEngine::GetRenderSystem();
        renderSystem.BindFramebuffer( m_pFramebuffer );
        renderSystem.Clear();

        if ( m_PrimaryCameraEntity )
        {
            const Camera &camera = m_PrimaryCameraEntity.GetComponent< ecs::CameraComponent >().Camera;
            auto transform = m_PrimaryCameraEntity.GetComponent< world::ecs::TransformComponent >().GetTransform();

            m_RenderPassList.OnRender( camera, transform );

            // TODO: Add scene graph
            SkyboxRenderer::BeginScene( camera, transform );
            SkyboxRenderer::OnRender();
            SkyboxRenderer::EndScene();
        }
        else if ( m_FallbackCameraData.pCamera )
        {
            m_RenderPassList.OnRender( *m_FallbackCameraData.pCamera, m_FallbackCameraData.CameraTransform );

            // TODO: Add scene graph
            SkyboxRenderer::BeginScene( *m_FallbackCameraData.pCamera, m_FallbackCameraData.CameraTransform );
            SkyboxRenderer::OnRender();
            SkyboxRenderer::EndScene();
        }

        renderSystem.BindBackBuffer();
    }

    void Scene::OnViewportResize( Uint32 width, Uint32 height )
    {
        if ( width == m_ViewportWidth && height == m_ViewportHeight )
            return;

        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
        resourceManager.ResizeFramebuffer( m_pFramebuffer, width, height );
    }
}