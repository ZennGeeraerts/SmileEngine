/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_pass_list.h"

namespace smile::graphic
{
    RenderPassList::~RenderPassList() noexcept
    {
        Clear();
    }

    void RenderPassList::Execute( const Framebuffer &framebuffer, const View &view ) noexcept
    {
        for ( auto pass : m_RenderPasses )
        {
            pass->BeginPass( view );
            pass->Execute( framebuffer );
            pass->EndPass();
        }
    }

    void RenderPassList::Clear() noexcept
    {
        m_RenderPasses.Clear();
        m_RenderPassMap.Clear();
    }
}