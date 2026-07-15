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
 * @file        render_graph_resource.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Resource handles and descriptors used by the render graph
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"
#include "smile/graphic/rhi/format.h"
#include "smile/graphic/renderer/resource/frame_buffer_attachment.h"

namespace smile::graphic
{
    /**
     * Opaque handle referencing a transient texture resource within a RenderGraph.
     * Handles are acquired via RenderGraphPassBuilder::CreateTexture.
     */
    struct RenderGraphResourceHandle final
    {
        Index Index = s_InvalidIndex;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return Index != s_InvalidIndex;
        }

        bool operator==( const RenderGraphResourceHandle &other ) const noexcept
        {
            return Index == other.Index;
        }

        bool operator!=( const RenderGraphResourceHandle &other ) const noexcept
        {
            return Index != other.Index;
        }
    };

    /**
     * Descriptor used to declare a transient texture resource in the render graph.
     */
    struct RenderGraphTextureDescriptor final
    {
        Uint32 Width = 0;
        Uint32 Height = 0;
        rhi::Format Format = rhi::Format::UNKNOWN;
    };

    /**
     * Internal node representing a named transient resource.
     * Populated by the graph during pass setup and resolved to a GPU resource during Compile().
     */
    struct RenderGraphResourceNode final
    {
        primitive::String Name;
        RenderGraphTextureDescriptor Desc;
        FramebufferAttachment ResolvedAttachment{};
    };
}
