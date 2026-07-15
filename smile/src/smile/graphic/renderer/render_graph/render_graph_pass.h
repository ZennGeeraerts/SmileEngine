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
 * @file        render_graph_pass.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Pass nodes, builder interface, and resource query interface for the render graph
 */
#pragma once

#include "render_graph_resource.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

#include <functional>

namespace smile::graphic
{
    class RenderContext;

    /**
     * Read-only view into the graph's resolved resources for a specific pass.
     * Passed to each pass's execute callback so it can retrieve its declared inputs/outputs.
     */
    class RenderGraphPassResources final
    {
      public:
        RenderGraphPassResources( const primitive::Vector< RenderGraphResourceNode > &resources,
            const Framebuffer &framebuffer ) noexcept
            : m_Resources{ resources }, m_Framebuffer{ framebuffer }
        {
        }

        /**
         * Returns the resolved texture for the given resource handle.
         * The handle must have been acquired via RenderGraphPassBuilder::CreateTexture
         * during this pass's setup or from a producer pass.
         */
        const Texture &GetTexture( RenderGraphResourceHandle handle ) const noexcept;

        /**
         * Returns the framebuffer assembled from this pass's declared color and depth outputs.
         * Invalid if the pass declared no outputs.
         */
        const Framebuffer &GetFramebuffer() const noexcept;

      private:
        const primitive::Vector< RenderGraphResourceNode > &m_Resources;
        const Framebuffer &m_Framebuffer;
    };

    /**
     * Builder passed to a pass's setup callback.
     * Used to declare the pass's transient resource creations, reads, and writes.
     */
    class RenderGraphPassBuilder final
    {
      public:
        RenderGraphPassBuilder( primitive::Vector< RenderGraphResourceNode > &resources,
            primitive::Vector< RenderGraphResourceHandle > &inputs,
            primitive::Vector< RenderGraphResourceHandle > &colorOutputs,
            RenderGraphResourceHandle &depthOutput ) noexcept;

        /**
         * Declares a new transient texture resource with the given name and descriptor.
         * If a resource with the same name already exists, the existing handle is returned.
         */
        [[nodiscard]] RenderGraphResourceHandle CreateTexture( const primitive::String &name,
            const RenderGraphTextureDescriptor &desc );

        /**
         * Declares that this pass reads the given resource (establishes a dependency).
         */
        void ReadTexture( RenderGraphResourceHandle handle );

        /**
         * Declares that this pass writes to the given resource as a color render target.
         */
        void WriteColor( RenderGraphResourceHandle handle );

        /**
         * Declares that this pass writes to the given resource as the depth/stencil target.
         */
        void WriteDepth( RenderGraphResourceHandle handle );

      private:
        primitive::Vector< RenderGraphResourceNode > &m_Resources;
        primitive::Vector< RenderGraphResourceHandle > &m_Inputs;
        primitive::Vector< RenderGraphResourceHandle > &m_ColorOutputs;
        RenderGraphResourceHandle &m_DepthOutput;
    };

    /**
     * Internal node storing a single pass's metadata and resolved GPU objects.
     * Created and owned by RenderGraph.
     */
    struct RenderGraphPassNode final
    {
        primitive::String Name;
        primitive::Vector< RenderGraphResourceHandle > Inputs;
        primitive::Vector< RenderGraphResourceHandle > ColorOutputs;
        RenderGraphResourceHandle DepthOutput;
        std::function< void( const RenderGraphPassResources &, RenderContext & ) > ExecuteFn;
        Framebuffer ResolvedFramebuffer{};
    };
}
