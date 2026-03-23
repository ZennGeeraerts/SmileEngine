/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_pass_list.h"

namespace smile::graphic
{
    RenderPassList::~RenderPassList()
    {
        Clear();
    }

    void RenderPassList::OnRender( Framebuffer::Ref framebuffer,
        const Camera &camera,
        const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        for ( auto pass : m_pRenderPasses )
        {
            pass->OnRender( framebuffer, camera, cameraTransform );
        }
    }

    void RenderPassList::PushBack( memory::Ref< RenderPass > pRenderPass )
    {
        primitive::array::EraseIfExists( m_pRenderPasses, pRenderPass );
        m_pRenderPasses.PushBack( pRenderPass );
    }

    void RenderPassList::PushFront( memory::Ref< RenderPass > pRenderPass )
    {
        primitive::array::EraseIfExists( m_pRenderPasses, pRenderPass );
        m_pRenderPasses.PushFront( pRenderPass );
    }

    void RenderPassList::Remove( memory::Ref< RenderPass > pRenderPass )
    {
        m_pRenderPasses.Erase( pRenderPass );
    }

    void RenderPassList::Clear()
    {
        m_pRenderPasses.Clear();
    }
}