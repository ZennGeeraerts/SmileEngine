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
        m_SpriteBatch.Initialize( rm );
    }

    void Renderer::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
        m_Engine.GetRenderContext().Open();
    }

    void Renderer::OnRender( RenderScene &scene )
    {
        auto &ctx = m_Engine.GetRenderContext();
        auto &resourceManager = m_Engine.GetResourceManager();

        // Flush batch systems into their targets
        m_SpriteBatch.Flush( scene, resourceManager, ctx ); // SceneLayer::Sprite Renderables
        m_DebugRenderer.Flush( m_DebugData );               // DebugPassData::LineList

        // Register render passes
        RenderGraphResourceHandle colorHandle;
        RenderGraphResourceHandle depthHandle;

        const auto &framebuffer = scene.GetFramebuffer();

        SM_ASSERT( framebuffer.IsValid() );

        AddForwardPass(
            m_Graph, m_ForwardData, scene, framebuffer.GetWidth(), framebuffer.GetHeight(), colorHandle, depthHandle );

        AddSpritePass( m_Graph, m_ForwardData, scene, colorHandle, depthHandle );
        AddDebugPass( m_Graph, m_DebugData, scene, colorHandle );

        // PostProcess - extension point (tone-map, bloom); currently a no-op passthrough
        m_Graph.AddPass(
            "PostProcess",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( colorHandle );
                builder.WriteColor( colorHandle );
            },
            []( const RenderGraphPassResources &, RenderContext & ) {} );

        AddUIPass( m_Graph, m_ForwardData, scene, colorHandle );

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
        m_SpriteBatch.Reset();
        m_DebugData.Reset();
        m_DebugRenderer.Reset();

        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }
}
