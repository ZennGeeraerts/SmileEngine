/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "smile_engine/graphic/renderer/forward_renderer.h"
#include "smile_engine/graphic/animation/ecs/animator_component.h"
#include "smile_engine/core/scene/ecs/transform_component.h"

namespace smile::graphic::ecs
{
    void ForwardRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        graphic::ForwardRenderer::BeginScene( camera, cameraTransform );

        {
            auto group =
                m_ECSEngine.GetGroup< MeshRendererComponent >( smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< MeshRendererComponent, scene::ecs::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( meshRenderer, transform.GetWorldTransform() );
            }
        }
        {
            // TODO: Remove dependency on animator
            auto group = m_ECSEngine.GetGroup< SkinnedMeshRendererComponent, AnimatorComponent >(
                smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[skinnedMeshRenderer, transform] =
                    m_ECSEngine.GetComponents< SkinnedMeshRendererComponent, scene::ecs::TransformComponent >( entity );

                graphic::ForwardRenderer::Submit( skinnedMeshRenderer, transform.GetWorldTransform() );
            }
        }

        graphic::ForwardRenderer::OnRender();

        graphic::ForwardRenderer::EndScene();
    }

    void ForwardRenderPass::OnRender( const EditorCamera &editorCamera )
    {
        graphic::ForwardRenderer::BeginScene( editorCamera );

        {
            auto group =
                m_ECSEngine.GetGroup< MeshRendererComponent >( smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< MeshRendererComponent, scene::ecs::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( meshRenderer, transform.GetWorldTransform() );
            }
        }
        {
            // TODO: Remove dependency on animator
            auto group = m_ECSEngine.GetGroup< SkinnedMeshRendererComponent, AnimatorComponent >(
                smile::ecs::g_Get< scene::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[skinnedMeshRenderer, transform] =
                    m_ECSEngine.GetComponents< SkinnedMeshRendererComponent, scene::ecs::TransformComponent >( entity );

                graphic::ForwardRenderer::Submit( skinnedMeshRenderer, transform.GetWorldTransform() );
            }
        }

        graphic::ForwardRenderer::OnRender();

        graphic::ForwardRenderer::EndScene();
    }
}