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
 * @brief       Per-frame orchestration — builds the RenderGraph each frame
 */
#include "smpch.h"
#include "renderer.h"

#include "frame.h"
#include "render_engine.h"
#include "render_world.h"
#include "draw/draw_command_buffer.h"
#include "smile/core/math/color.h"
#include "smile/graphic/renderer/render_graph/render_graph_resource.h"

namespace smile::graphic
{
    Renderer::Renderer( RenderEngine &engine ) noexcept : m_Engine{ engine }, m_Graph{ engine.GetResourceManager() }
    {
        auto &ctx = engine.GetRenderContext();
        auto &rm = engine.GetResourceManager();

        m_ForwardData.Initialize( ctx, rm, engine.GetMaterialSystem() );
        m_DebugData.Initialize( ctx, rm, engine.GetShaderLibrary() );
    }

    void Renderer::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
        m_Engine.GetRenderContext().Open();
    }

    void Renderer::OnRender( RenderWorld &renderWorld,
        DrawCommandBuffer &buffer,
        const View &view,
        const Framebuffer &framebuffer )
    {
        auto &ctx = m_Engine.GetRenderContext();
        auto &resourceManager = m_Engine.GetResourceManager();

        m_DebugRenderer.Flush( m_DebugData ); // DebugPassData::LineList

        // Register render passes
        RenderGraphResourceHandle colorHandle;
        RenderGraphResourceHandle depthHandle;

        SM_ASSERT( framebuffer.IsValid() );

        AddForwardPass( m_Graph,
            m_ForwardData,
            renderWorld,
            buffer,
            framebuffer.GetWidth(),
            framebuffer.GetHeight(),
            colorHandle,
            depthHandle );

        AddDebugPass( m_Graph, m_DebugData, view, colorHandle );

        // PostProcess - extension point (tone-map, bloom); currently a no-op passthrough
        m_Graph.AddPass(
            "PostProcess",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( colorHandle );
                builder.WriteColor( colorHandle );
            },
            []( const RenderGraphPassResources &, RenderContext & ) {} );

        // PresentPass - blit SceneColor to final render target
        m_Graph.AddPass(
            "PresentPass",
            [&]( RenderGraphPassBuilder &builder ) { builder.ReadTexture( colorHandle ); },
            [&ctx, &resourceManager, &framebuffer, colorHandle]( const RenderGraphPassResources &res, RenderContext & )
            {
                const FramebufferAttachmentSet &fbSet = resourceManager.GetFramebufferAttachmentSet( framebuffer );

                SM_ASSERT( !fbSet.ColorAttachments.IsEmpty() );

                ctx.CopyTexture( fbSet.ColorAttachments[0].Texture, {}, res.GetTexture( colorHandle ), {} );
            } );

        m_Graph.Compile();
        m_Graph.Execute( ctx );
    }

    void Renderer::EndFrame( const rhi::SwapChain &swapChain )
    {
        auto &ctx = m_Engine.GetRenderContext();
        auto &rm = m_Engine.GetResourceManager();

        swapChain.Present();
        ctx.Close();

        m_Graph.Reset();
        m_DebugData.Reset();
        m_DebugRenderer.Reset();

        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }
}
