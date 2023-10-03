/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_pass_2d.h"

#include "smile_engine/graphic/renderer/renderer_2d.h"

namespace smile::graphic
{
    void RenderPass2D::OnRender()
    {
        Camera *pMainCamera = nullptr;
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
            Renderer2D::BeginScene( *pMainCamera, cameraTransform );

            {
                auto group =
                    m_ECSEngine.GetGroup< scene::SpriteRendererComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[spriteRenderer, transform] =
                        m_ECSEngine.GetComponents< scene::SpriteRendererComponent, scene::TransformComponent >(
                            entity );
                    Renderer2D::DrawQuad( transform.GetTransform(), spriteRenderer );
                }
            }

            Renderer2D::OnRender();

            Renderer2D::EndScene();
        }
    }

    void RenderPass2D::OnRender( const EditorCamera &editorCamera )
    {
        Renderer2D::BeginScene( editorCamera );

        {
            auto group =
                m_ECSEngine.GetGroup< scene::SpriteRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[spriteRenderer, transform] =
                    m_ECSEngine.GetComponents< scene::SpriteRendererComponent, scene::TransformComponent >( entity );
                Renderer2D::DrawQuad( transform.GetTransform(), spriteRenderer );
            }
        }

        Renderer2D::OnRender();

        Renderer2D::EndScene();
    }
}