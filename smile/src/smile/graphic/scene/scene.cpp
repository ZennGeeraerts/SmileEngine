/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/skybox_renderer.h"

#include "window/window.h"

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

        const RenderEngine::CameraData &cameraData = RenderEngine::GetCameraData();

        if ( cameraData.pMainCamera )
        {
            m_RenderPassList.OnRender( *cameraData.pMainCamera, cameraData.CameraTransform );

            SkyboxRenderer::BeginScene( *cameraData.pMainCamera, cameraData.CameraTransform );
            SkyboxRenderer::OnRender();
            SkyboxRenderer::EndScene();
        }

        renderSystem.BindBackBuffer();
    }
}