/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_pass_2d.h"

#include "smile_engine/graphic/renderer/renderer_2d.h"
#include "smile_engine/core/scene/ecs/transform_component.h"

namespace smile::graphic::ecs
{
    void RenderPass2D::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        Renderer2D::BeginScene( camera, cameraTransform );

        {
            auto group =
                m_ECSEngine.GetGroup< SpriteRendererComponent >( smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[spriteRenderer, transform] =
                    m_ECSEngine.GetComponents< SpriteRendererComponent, scene::ecs::TransformComponent >( entity );
                Renderer2D::DrawQuad( transform.GetWorldTransform(), spriteRenderer );
            }
        }

        Renderer2D::OnRender();

        Renderer2D::EndScene();
    }

    void RenderPass2D::OnRender( const EditorCamera &editorCamera )
    {
        Renderer2D::BeginScene( editorCamera );

        {
            auto group =
                m_ECSEngine.GetGroup< SpriteRendererComponent >( smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[spriteRenderer, transform] =
                    m_ECSEngine.GetComponents< SpriteRendererComponent, scene::ecs::TransformComponent >( entity );
                Renderer2D::DrawQuad( transform.GetWorldTransform(), spriteRenderer );
            }
        }

        Renderer2D::OnRender();

        Renderer2D::EndScene();
    }
}