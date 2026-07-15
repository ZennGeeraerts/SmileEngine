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
 * @file        transient_texture_pool.h
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       Per-frame transient render-target pool for the RenderGraph
 */
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/format.h"
#include "smile/graphic/renderer/resource/frame_buffer_attachment.h"

namespace smile::graphic
{
    class ResourceManager;

    /**
     * Cache key identifying a class of compatible render-target textures.
     */
    struct TransientTextureKey final
    {
        Uint32 Width = 0;
        Uint32 Height = 0;
        rhi::Format Format = rhi::Format::UNKNOWN;

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Width ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Height ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( static_cast< Uint32 >( Format ) ) );
            return hash;
        }

        bool operator==( const TransientTextureKey &other ) const noexcept
        {
            return Width == other.Width && Height == other.Height && Format == other.Format;
        }
    };

    /**
     * Recycles transient render-target textures across frames to avoid per-frame GPU allocations.
     *
     * Each frame, passes call Acquire() to get a texture; at frame end Reset() calls Release()
     * on each resolved attachment to return it to the free list. Textures are only actually
     * destroyed when Flush() is called (e.g. on swap-chain resize or engine shutdown).
     */
    class TransientTexturePool final
    {
      public:
        explicit TransientTexturePool( ResourceManager &resourceManager ) noexcept;
        ~TransientTexturePool() = default;

        TransientTexturePool( const TransientTexturePool & ) = delete;
        TransientTexturePool &operator=( const TransientTexturePool & ) = delete;

        /**
         * Returns a texture matching the requested dimensions and format.
         * Reuses a pooled texture when one is available; otherwise allocates a new one
         * via ResourceManager.
         */
        FramebufferAttachment Acquire( Uint32 width, Uint32 height, rhi::Format format );

        /**
         * Returns a texture to the pool. No GPU resources are freed.
         * Call once per acquired attachment at end of each frame.
         */
        void Release( const FramebufferAttachment &attachment );

        /**
         * Destroys all pooled textures via ResourceManager. Must be called before the
         * ResourceManager or device are torn down, and whenever the resolution changes.
         */
        void Flush();

      private:
        primitive::HashMap< TransientTextureKey, primitive::Vector< FramebufferAttachment > > m_FreeList;
        ResourceManager &m_ResourceManager;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::TransientTextureKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::TransientTextureKey &key ) const noexcept
        {
            return key.GetHashCode();
        }
    };
}
