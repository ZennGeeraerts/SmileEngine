/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "smile_engine/graphic/renderer/forward_renderer.h"

namespace smile::graphic
{
    void ForwardRenderPass::OnRender()
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
            graphic::ForwardRenderer::BeginScene( *pMainCamera, cameraTransform );

            {
                auto group =
                    m_ECSEngine.GetGroup< scene::MeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] =
                        m_ECSEngine.GetComponents< scene::MeshRendererComponent, scene::TransformComponent >( entity );
                    graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group =
                    m_ECSEngine.GetGroup< scene::StaticMeshComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] =
                        m_ECSEngine.GetComponents< scene::StaticMeshComponent, scene::TransformComponent >( entity );
                    graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group =
                    m_ECSEngine.GetGroup< scene::SkinnedMeshComponent >( ecs::g_Get< scene::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] =
                        m_ECSEngine.GetComponents< scene::SkinnedMeshComponent, scene::TransformComponent >( entity );

                    graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
                }
            }

            graphic::ForwardRenderer::OnRender();

            graphic::ForwardRenderer::EndScene();
        }
    }

    void ForwardRenderPass::OnRender( const EditorCamera &editorCamera )
    {
        graphic::ForwardRenderer::BeginScene( editorCamera );

        {
            auto group =
                m_ECSEngine.GetGroup< scene::MeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] =
                    m_ECSEngine.GetComponents< scene::MeshRendererComponent, scene::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< scene::StaticMeshComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] =
                    m_ECSEngine.GetComponents< scene::StaticMeshComponent, scene::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< scene::SkinnedMeshComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] =
                    m_ECSEngine.GetComponents< scene::SkinnedMeshComponent, scene::TransformComponent >( entity );

                graphic::ForwardRenderer::Submit( mesh, transform.GetTransform() );
            }
        }

        graphic::ForwardRenderer::OnRender();

        graphic::ForwardRenderer::EndScene();
    }
}