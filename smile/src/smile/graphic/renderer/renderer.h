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
 * @brief       Per-frame orchestration. Owns the RenderGraph (DAG + resource lifetimes),
 *              pass data structs, and batch systems. Registers passes via free functions
 *              in OnRender() — no long-lived pass objects, no virtual dispatch.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/renderer/render_graph/render_graph.h"
#include "smile/graphic/renderer/passes/forward_pass.h"
#include "smile/graphic/renderer/passes/debug_pass.h"
#include "smile/graphic/renderer/sprite/sprite_batch.h"
#include "smile/graphic/renderer/debug_renderer.h"
#include "smile/graphic/rhi/swap_chain.h"

namespace smile::graphic
{
    class RenderEngine;

    class Renderer final
    {
      public:
        explicit Renderer( RenderEngine &engine ) noexcept;
        ~Renderer() = default;

        Renderer( const Renderer & ) = delete;
        Renderer( Renderer && ) = delete;
        Renderer &operator=( const Renderer & ) = delete;
        Renderer &operator=( Renderer && ) = delete;

        /** Opens the command list */
        void BeginFrame();

        /**
         * Flushes CPU batches, registers all render passes into the RenderGraph,
         * compiles, and executes in dependency order.
         */
        void OnRender( RenderScene &scene );

        /** Presents, closes the command list, and resets all per-frame state. */
        void EndFrame( const rhi::SwapChain &swapChain );

        // ---- Batch submission (called from SceneExtractor / gameplay) ----

        SpriteBatch &GetSpriteBatch() noexcept
        {
            return m_SpriteBatch;
        }

        DebugRenderer &GetDebugRenderer() noexcept
        {
            return m_DebugRenderer;
        }

      private:
        RenderEngine &m_Engine;

        RenderGraph m_Graph;
        ForwardPassData m_ForwardData;
        DebugPassData m_DebugData;
        SpriteBatch m_SpriteBatch;
        DebugRenderer m_DebugRenderer;

        Index m_CurrentFrameIndex{ 0 };
        Index m_RenderedFrameIndex{ 0 };
    };
}