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
 * @file        render_graph.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Render graph — declares pass dependencies, manages transient resources,
 *              and executes passes in dependency order
 */
#pragma once

#include "render_graph_pass.h"
#include "transient_texture_pool.h"
#include "smile/common/primitive/collection/vector.h"

namespace smile::graphic
{
    class ResourceManager;
    class RenderContext;

    /**
     * Render graph that collects render passes, resolves their resource dependencies,
     * and drives execution each frame.
     *
     * Typical per-frame usage:
     * @code
     *   RenderGraphResourceHandle colorHandle;
     *
     *   graph.AddPass("ForwardPass",
     *       [&](RenderGraphPassBuilder& builder)
     *       {
     *           RenderGraphTextureDesc colorDesc{ width, height, rhi::Format::RGBA8_UNORM, false };
     *           RenderGraphTextureDesc depthDesc{ width, height, rhi::Format::D32,         true  };
     *           colorHandle = builder.CreateTexture("SceneColor", colorDesc);
     *           auto depth  = builder.CreateTexture("SceneDepth", depthDesc);
     *           builder.WriteColor(colorHandle);
     *           builder.WriteDepth(depth);
     *       },
     *       [&](const RenderGraphPassResources& res, RenderContext& ctx)
     *       {
     *           // render into res.GetFramebuffer()
     *       });
     *
     *   graph.AddPass("PostProcess",
     *       [&](RenderGraphPassBuilder& builder)
     *       {
     *           builder.ReadTexture(colorHandle);
     *           // ...
     *       },
     *       [&](const RenderGraphPassResources& res, RenderContext& ctx)
     *       {
     *           auto& sceneColor = res.GetTexture(colorHandle);
     *           // ...
     *       });
     *
     *   graph.Compile(resourceManager);
     *   graph.Execute(renderContext);
     *   graph.Reset(resourceManager); // end of frame — release transient resources
     * @endcode
     */
    class RenderGraph final
    {
      public:
        explicit RenderGraph( ResourceManager &resourceManager ) noexcept;
        ~RenderGraph();

        RenderGraph( const RenderGraph & ) = delete;
        RenderGraph( RenderGraph && ) = delete;
        RenderGraph &operator=( const RenderGraph & ) = delete;
        RenderGraph &operator=( RenderGraph && ) = delete;

        /**
         * Registers a pass with the graph.
         * @param name     Debug name for the pass.
         * @param setupFn  void(RenderGraphPassBuilder&) — declares the pass's resource reads/writes.
         *                 Called immediately so that returned handles can be captured for later passes.
         * @param executeFn void(const RenderGraphPassResources&, RenderContext&) — rendering work.
         *                  Called during Execute(), after Compile() has created all GPU resources.
         */
        template < typename SetupFn, typename ExecuteFn >
        void AddPass( const primitive::String &name, SetupFn &&setupFn, ExecuteFn &&executeFn )
        {
            m_Passes.PushBack();
            auto &pass = m_Passes.GetLastItem();
            pass.Name = name;
            pass.ExecuteFn = std::forward< ExecuteFn >( executeFn );

            RenderGraphPassBuilder builder{ m_Resources, pass.Inputs, pass.ColorOutputs, pass.DepthOutput };
            std::forward< SetupFn >( setupFn )( builder );
        }

        /**
         * Creates GPU resources (textures, framebuffers) for all declared transient resources.
         * Must be called once after all passes have been added and before Execute().
         */
        void Compile();

        /**
         * Executes all passes in the order they were added.
         * Compile() must have been called first.
         */
        void Execute( RenderContext &context );

        /**
         * Destroys all transient GPU resources and clears the pass list.
         * Call at the end of the frame to prepare the graph for re-use.
         */
        void Reset();

        /**
         * Destroys all pooled transient textures. Call on swap-chain resize and engine shutdown.
         */
        void FlushPool();

      private:
        /**
         * Runs Kahn's algorithm over the pass->resource->pass dependency edges.
         * Fills m_SortedOrder with pass indices in topological (execution) order.
         * Logs a warning and falls back to declaration order on a cycle.
         */
        void TopologicalSort();

      private:
        primitive::Vector< RenderGraphPassNode > m_Passes;
        primitive::Vector< RenderGraphResourceNode > m_Resources;
        primitive::Vector< Index > m_SortedOrder; ///< Pass indices in execution order after Compile()
        TransientTexturePool m_TexturePool;
        bool m_IsCompiled = false;

        ResourceManager &m_ResourceManager;
    };
}
