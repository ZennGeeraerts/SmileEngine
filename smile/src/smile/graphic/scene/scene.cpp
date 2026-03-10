/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/scene/skybox_renderer.h"
#include "ecs/camera_component.h"

#include "smile/core/window/window.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    Scene::Scene( const window::Window *pWindow )
    {
        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

        auto width = pWindow->GetWidth();
        auto height = pWindow->GetHeight();

        FramebufferAttachment colorAttachment = resourceManager.CreateColorAttachment( width, height );
        FramebufferAttachment depthAttachment = resourceManager.CreateDepthAttachment( width, height );
        FramebufferAttachment colorAttachment2 = resourceManager.CreateColorAttachment( width, height );

        m_pFramebuffer = resourceManager.CreateFramebuffer( { colorAttachment, colorAttachment2 }, depthAttachment );
    }

    void Scene::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        m_RenderPassList.OnAdd( ecsEngine );
    }

    void Scene::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        m_RenderPassList.OnRemove( ecsEngine );
    }

    void Scene::OnRender()
    {
        auto &renderSystem = RenderEngine::GetRenderSystem();

        auto currentFramebuffer = m_RenderToSwapChain ? renderSystem.GetBackBuffer() : m_pFramebuffer;

        renderSystem.Clear( currentFramebuffer, std::nullopt, std::nullopt, std::nullopt );

        if ( m_PrimaryCameraEntity )
        {
            const Camera &camera = m_PrimaryCameraEntity.GetComponent< ecs::CameraComponent >().Camera;
            auto transform = m_PrimaryCameraEntity.GetComponent< world::ecs::TransformComponent >().GetTransform();

            m_RenderPassList.OnRender( currentFramebuffer,
                camera,
                transform ); // TODO: Blit framebuffer texture to swapchain texture

            // TODO: Add scene graph
            SkyboxRenderer::BeginScene( camera, transform );
            SkyboxRenderer::OnRender( currentFramebuffer );
            SkyboxRenderer::EndScene();
        }
        else if ( m_FallbackCameraData.pCamera )
        {
            m_RenderPassList.OnRender(
                currentFramebuffer, *m_FallbackCameraData.pCamera, m_FallbackCameraData.CameraTransform );

            // TODO: Add scene graph
            SkyboxRenderer::BeginScene( *m_FallbackCameraData.pCamera, m_FallbackCameraData.CameraTransform );
            SkyboxRenderer::OnRender( currentFramebuffer );
            SkyboxRenderer::EndScene();
        }
    }

    void *Scene::GetFinalColor() const
    {
        const auto &colorAttachments = m_pFramebuffer->GetColorAttachments();

        return RenderEngine::GetRenderSystem().GetResourceManager().GetShaderResourceView(
            colorAttachments[0].pTexture );
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