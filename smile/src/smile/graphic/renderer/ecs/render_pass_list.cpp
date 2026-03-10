/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_pass_list.h"

namespace smile::graphic::ecs
{
    void RenderPassList::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        m_pECSEngine = &ecsEngine;

        for ( const auto &pRenderPass : m_pRenderPasses )
            pRenderPass->OnAdd( ecsEngine );
    }

    void RenderPassList::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        for ( const auto &pRenderPass : m_pRenderPasses )
            pRenderPass->OnRemove( ecsEngine );

        m_pECSEngine = nullptr;
    }

    void RenderPassList::OnRender( Framebuffer::Ref framebuffer,
        const Camera &camera,
        const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        for ( auto &pRenderPass : m_pRenderPasses )
            pRenderPass->OnRender( camera, cameraTransform );
    }

    void RenderPassList::Add( memory::Ref< RenderPass > pRenderPass )
    {
        m_pRenderPasses.push_back( pRenderPass );

        if ( m_pECSEngine )
            pRenderPass->OnAdd( *m_pECSEngine );
    }
}