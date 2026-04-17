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
 * @file        renderer.cpp
 * @author      Zenn Geeraerts
 * @created     17 April 2026
 * @brief       Render pass orchastration and frame management
 */
#include "smpch.h"
#include "renderer.h"

#include "frame.h"
#include "render_engine.h"

namespace smile::graphic
{
    void Renderer::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
        RenderEngine::GetRenderContext().Open();
    }

    void Renderer::OnRender( const View &view, Framebuffer::Ref framebuffer )
    {
        if ( !framebuffer )
        {
            framebuffer = RenderEngine::GetRenderContext().GetBackBuffer();
        }

        RenderEngine::GetRenderContext().Clear(
            framebuffer, math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        m_RenderPassList.Execute( framebuffer, view );
    }

    void Renderer::EndFrame()
    {
        auto &context = RenderEngine::GetRenderContext();

        context.Present();
        context.Close();
        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }
}