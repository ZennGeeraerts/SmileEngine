/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "forward_render_pass.h"

#include "engine/graphic/renderer/forward_renderer.h"
#include "engine/graphic/animation/ecs/animator_component.h"
#include "world/ecs/transform_component.h"

namespace smile::graphic::ecs
{
    void ForwardRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        graphic::ForwardRenderer::BeginScene( camera, cameraTransform );

        {
            auto group =
                m_ECSEngine.GetGroup< MeshRendererComponent >( smile::ecs::g_Get< world::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< MeshRendererComponent, world::ecs::TransformComponent >( entity );
                graphic::ForwardRenderer::Submit( meshRenderer, transform.GetWorldTransform() );
            }
        }
        {
            // TODO: Remove dependency on animator
            auto group = m_ECSEngine.GetGroup< SkinnedMeshRendererComponent, AnimatorComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[skinnedMeshRenderer, transform] =
                    m_ECSEngine.GetComponents< SkinnedMeshRendererComponent, world::ecs::TransformComponent >( entity );

                graphic::ForwardRenderer::Submit( skinnedMeshRenderer, transform.GetWorldTransform() );
            }
        }

        graphic::ForwardRenderer::OnRender();

        graphic::ForwardRenderer::EndScene();
    }
}