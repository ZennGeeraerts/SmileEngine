/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "wireframe_render_pass.h"

#include "smile/graphic/renderer/wireframe_renderer.h"

namespace smile::graphic::ecs
{
    void WireframeRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        WireframeRenderer &wireframeRenderer = WireframeRenderer::GetInstance();

        wireframeRenderer.BeginScene( camera, cameraTransform );

        {
            auto group = m_ECSEngine.GetGroup< physics::ecs::BoxColliderComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] =
                    m_ECSEngine.GetComponents< physics::ecs::BoxColliderComponent, world::ecs::TransformComponent >(
                        entity );
                wireframeRenderer.Submit( boxCollider, transform.GetWorldTransform() );
            }
        }

        {
            auto group = m_ECSEngine.GetGroup< physics::ecs::SphereColliderComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[sphereCollider, transform] =
                    m_ECSEngine.GetComponents< physics::ecs::SphereColliderComponent, world::ecs::TransformComponent >(
                        entity );
                wireframeRenderer.Submit( sphereCollider, transform.GetWorldTransform() );
            }
        }

        wireframeRenderer.OnRender();

        wireframeRenderer.EndScene();
    }
}