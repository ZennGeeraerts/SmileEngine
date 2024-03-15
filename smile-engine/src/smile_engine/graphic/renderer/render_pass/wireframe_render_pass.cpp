/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "wireframe_render_pass.h"

#include "smile_engine/graphic/renderer/wireframe_renderer.h"

namespace smile::graphic
{
    void WireframeRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        WireframeRenderer &wireframeRenderer = WireframeRenderer::GetInstance();

        wireframeRenderer.BeginScene( camera, cameraTransform );

        {
            auto group = m_ECSEngine.GetGroup< scene::BoxColliderComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] =
                    m_ECSEngine.GetComponents< scene::BoxColliderComponent, scene::TransformComponent >( entity );
                wireframeRenderer.Submit( boxCollider, transform.GetWorldTransform() );
            }
        }

        {
            auto group =
                m_ECSEngine.GetGroup< scene::SphereColliderComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[sphereCollider, transform] =
                    m_ECSEngine.GetComponents< scene::SphereColliderComponent, scene::TransformComponent >( entity );
                wireframeRenderer.Submit( sphereCollider, transform.GetWorldTransform() );
            }
        }

        wireframeRenderer.OnRender();

        wireframeRenderer.EndScene();
    }

    void WireframeRenderPass::OnRender( const EditorCamera &editorCamera )
    {
        WireframeRenderer &wireframeRenderer = WireframeRenderer::GetInstance();

        wireframeRenderer.BeginScene( editorCamera );

        {
            auto group = m_ECSEngine.GetGroup< scene::BoxColliderComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] =
                    m_ECSEngine.GetComponents< scene::BoxColliderComponent, scene::TransformComponent >( entity );
                wireframeRenderer.Submit( boxCollider, transform.GetWorldTransform() );
            }
        }

        {
            auto group =
                m_ECSEngine.GetGroup< scene::SphereColliderComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[sphereCollider, transform] =
                    m_ECSEngine.GetComponents< scene::SphereColliderComponent, scene::TransformComponent >( entity );
                wireframeRenderer.Submit( sphereCollider, transform.GetWorldTransform() );
            }
        }

        wireframeRenderer.OnRender();

        wireframeRenderer.EndScene();
    }
}