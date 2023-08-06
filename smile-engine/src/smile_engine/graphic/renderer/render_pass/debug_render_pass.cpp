/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_render_pass.h"

#include "smile_engine/graphic/renderer/debug_renderer.h"
#include "smile_engine/scene/components.h"

namespace smile::graphic
{
    void DebugRenderPass::OnRender()
    {
        graphic::Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform;
        {
            auto view = m_ECSEngine.GetView< scene::TransformComponent, scene::CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] =
                    m_ECSEngine.GetComponents< scene::TransformComponent, scene::CameraComponent >( entity );

                if ( camera.IsPrimary )
                {
                    pMainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if ( pMainCamera )
        {
            auto &debugRenderer = DebugRenderer::GetInstance();
            debugRenderer.BeginScene( *pMainCamera, cameraTransform );
            debugRenderer.OnRender();
            debugRenderer.EndScene();
        }
    }

    void DebugRenderPass::OnRender( const EditorCamera &editorCamera )
    {
        auto &debugRenderer = DebugRenderer::GetInstance();
        debugRenderer.BeginScene( editorCamera );
        debugRenderer.OnRender();
        debugRenderer.EndScene();
    }
}