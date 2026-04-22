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
    Renderer::Renderer( RenderEngine &engine, RenderContext &context ) noexcept
        : m_Engine{ engine }, m_Context{ context }
    {
    }

    void Renderer::BeginFrame( rhi::SwapChain *swapChain )
    {
        SM_ASSERT( swapChain );

        m_SwapChain = swapChain;

        m_CurrentFrameIndex = AssignFrameData();
        m_Context.Open();
    }

    void Renderer::OnRender( const View &view, Framebuffer::Ref framebuffer )
    {
        SM_ASSERT( m_SwapChain );

        if ( !framebuffer )
        {
            framebuffer = m_Engine.GetRenderTarget( m_SwapChain );
        }

        m_Context.Clear( framebuffer, math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        m_RenderPassList.Execute( framebuffer, view );
    }

    void Renderer::EndFrame()
    {
        SM_ASSERT( m_SwapChain );

        m_SwapChain->Present();
        m_Context.Close();
        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;

        m_SwapChain = nullptr;
    }
}