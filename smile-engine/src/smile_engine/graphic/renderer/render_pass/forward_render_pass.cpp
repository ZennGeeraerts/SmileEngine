/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "smile_engine/graphic/renderer/forward_renderer.h"

namespace smile::graphic
{
    void ForwardRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        graphic::ForwardRenderer::BeginScene( camera, cameraTransform );

        {
            auto group =
                m_ECSEngine.GetGroup< scene::MeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< scene::MeshRendererComponent, scene::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( meshRenderer, transform.GetTransform() );
            }
        }
        {
            auto group =
                m_ECSEngine.GetGroup< scene::SkinnedMeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[skinnedMeshRenderer, transform] =
                    m_ECSEngine.GetComponents< scene::SkinnedMeshRendererComponent, scene::TransformComponent >(
                        entity );

                graphic::ForwardRenderer::Submit( skinnedMeshRenderer, transform.GetTransform() );
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
                m_ECSEngine.GetGroup< scene::MeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< scene::MeshRendererComponent, scene::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( meshRenderer, transform.GetTransform() );
            }
        }
        {
            auto group =
                m_ECSEngine.GetGroup< scene::SkinnedMeshRendererComponent >( ecs::g_Get< scene::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[skinnedMeshRenderer, transform] =
                    m_ECSEngine.GetComponents< scene::SkinnedMeshRendererComponent, scene::TransformComponent >(
                        entity );

                graphic::ForwardRenderer::Submit( skinnedMeshRenderer, transform.GetTransform() );
            }
        }

        graphic::ForwardRenderer::OnRender();

        graphic::ForwardRenderer::EndScene();
    }
}