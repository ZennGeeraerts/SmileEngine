/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "engine/graphic/renderer/render_command.h"
#include "engine/graphic/renderer/render_engine.h"
#include "engine/graphic/renderer/skybox_renderer.h"

namespace smile::graphic
{
    void Scene::OnRender()
    {
        auto pContext = RenderCommand::GetGraphicsContext();
        pContext->ClearFramebuffer( m_pFramebuffer );
        pContext->BindFramebuffer( m_pFramebuffer );

        const RenderEngine::CameraData &cameraData = RenderEngine::GetCameraData();

        if ( cameraData.pMainCamera )
        {
            m_RenderPassList.OnRender( *cameraData.pMainCamera, cameraData.CameraTransform );

            SkyboxRenderer::BeginScene( *cameraData.pMainCamera, cameraData.CameraTransform );
            SkyboxRenderer::OnRender();
            SkyboxRenderer::EndScene();
        }

        pContext->UnbindFramebuffer();
    }
}