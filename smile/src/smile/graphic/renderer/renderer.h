/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        renderer.h
 * @author      Zenn Geeraerts
 * @created     17 April 2026
 * @brief       Render pass orchastration and frame management
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "render_pass/render_pass_list.h"
#include "smile/graphic/rhi/swap_chain.h"

namespace smile::graphic
{
    class RenderEngine;
    class RenderContext;

    class Renderer final
    {
      public:
        Renderer( RenderEngine &engine, RenderContext &context ) noexcept;
        ~Renderer() = default;

        void BeginFrame( rhi::SwapChain *swapChain );
        void OnRender( const View &view, const Framebuffer &framebuffer );
        void EndFrame();

        RenderPassList &GetRenderPassList()
        {
            return m_RenderPassList;
        }

      private:
        RenderEngine &m_Engine;
        RenderContext &m_Context;

        RenderPassList m_RenderPassList;

        rhi::SwapChain *m_SwapChain{ nullptr };

        Index m_CurrentFrameIndex{ 0 };
        Index m_RenderedFrameIndex{ 0 };
    };
}