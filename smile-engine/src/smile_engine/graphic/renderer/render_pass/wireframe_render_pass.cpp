/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "wireframe_render_pass.h"

#include "smile_engine/graphic/renderer/wireframe_renderer.h"

namespace smile::graphic
{
    void WireframeRenderPass::OnRender()
    {
        WireframeRenderer &wireframeRenderer = WireframeRenderer::GetInstance();

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
            wireframeRenderer.BeginScene( *pMainCamera, cameraTransform );

            {
                auto group =
                    m_ECSEngine.GetGroup< scene::BoxColliderComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[boxCollider, transform] =
                        m_ECSEngine.GetComponents< scene::BoxColliderComponent, scene::TransformComponent >( entity );
                    wireframeRenderer.Submit( boxCollider, transform.GetTransform() );
                }
            }

            {
                auto group =
                    m_ECSEngine.GetGroup< scene::SphereColliderComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[sphereCollider, transform] =
                        m_ECSEngine.GetComponents< scene::SphereColliderComponent, scene::TransformComponent >( entity );
                    wireframeRenderer.Submit( sphereCollider, transform.GetTransform() );
                }
            }

            wireframeRenderer.OnRender();

            wireframeRenderer.EndScene();
        }
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
                wireframeRenderer.Submit( boxCollider, transform.GetTransform() );
            }
        }

        {
            auto group =
                m_ECSEngine.GetGroup< scene::SphereColliderComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[sphereCollider, transform] =
                    m_ECSEngine.GetComponents< scene::SphereColliderComponent, scene::TransformComponent >( entity );
                wireframeRenderer.Submit( sphereCollider, transform.GetTransform() );
            }
        }

        wireframeRenderer.OnRender();

        wireframeRenderer.EndScene();
    }
}