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

namespace smile::graphic
{
    class Renderer final
    {
      public:
        Renderer() = default;
        ~Renderer() = default;

        void BeginFrame();
        void OnRender( const View &view, Framebuffer::Ref framebuffer = nullptr );
        void EndFrame();

        RenderPassList &GetRenderPassList()
        {
            return m_RenderPassList;
        }

      private:
        RenderPassList m_RenderPassList;

        Index m_CurrentFrameIndex{ 0 };
        Index m_RenderedFrameIndex{ 0 };
    };
}