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
 * @file        frame_arena.h
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Linear bump-pointer arena scoped to a single render frame.
 *
 *              The arena owns a list of aligned heap chunks and bumps a write cursor
 *              within the current chunk on every allocation.  When the current chunk
 *              cannot satisfy a request a new chunk (large enough to fit it) is
 *              appended.  Reset() rewinds every chunk to offset zero in O(chunks);
 *              the chunks themselves are kept so subsequent frames reuse the memory
 *              without going back to the heap.
 *
 *              Lifetime model: one arena per in-flight frame. The Renderer rotates
 *              between arenas externally — the arena class itself knows nothing about
 *              frames-in-flight.
 *
 *              Thread safety: not thread-safe. Allocate from one thread per arena.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/array_view.h"
#include "smile/common/primitive/collection/vector.h"

namespace smile::graphic
{
    /**
     * Bump-pointer arena that grows by adding aligned chunks on demand and rewinds
     * on Reset().  All allocations within an arena are released together.
     */
    class FrameArena final
    {
      public:
        /** Default chunk size (64 KB). Large enough for several thousand small allocations. */
        static constexpr Count s_DefaultChunkSize = 64u * 1024u;

        /**
         * @param initialChunkSize  Size of the first heap chunk, in bytes. Subsequent
         *                          chunks are at least this size; oversized allocations
         *                          get their own chunk sized to fit.
         */
        explicit FrameArena( const Count initialChunkSize = s_DefaultChunkSize ) noexcept;

        ~FrameArena();

        FrameArena( const FrameArena & ) = delete;
        FrameArena( FrameArena && ) = delete;
        FrameArena &operator=( const FrameArena & ) = delete;
        FrameArena &operator=( FrameArena && ) = delete;

        /**
         * Returns aligned raw bytes from the arena. The returned pointer is valid
         * until the next Reset() call.
         */
        [[nodiscard]] Byte *AllocateBytes( const Count size, const Count alignment );

        /**
         * Returns an array view of @p count default-initialised @p ItemType objects.
         * @p ItemType must be trivially destructible — Reset() will not run destructors.
         */
        template < typename ItemType >
        [[nodiscard]] primitive::ArrayView< ItemType > Allocate( const Count count )
        {
            static_assert(
                std::is_trivially_destructible_v< ItemType >, "FrameArena items must be trivially destructible" );

            if ( count == 0 )
                return {};

            Byte *bytes = AllocateBytes( sizeof( ItemType ) * count, alignof( ItemType ) );
            ItemType *items = reinterpret_cast< ItemType * >( bytes );

            // Default-initialise (no-op for trivial types but explicit for non-trivial trivially-destructible types).
            for ( Count index = 0; index < count; ++index )
                new ( items + index ) ItemType{};

            return { items, count };
        }

        /**
         * Copies @p items into the arena and returns a view of the copies.
         */
        template < typename ItemType >
        [[nodiscard]] primitive::ArrayView< ItemType > EmplaceSpan( std::initializer_list< ItemType > items )
        {
            const Count count = static_cast< Count >( items.size() );
            if ( count == 0 )
                return {};

            Byte *bytes = AllocateBytes( sizeof( ItemType ) * count, alignof( ItemType ) );
            ItemType *dst = reinterpret_cast< ItemType * >( bytes );

            Index index = 0;
            for ( const ItemType &item : items )
            {
                new ( dst + index ) ItemType{ item };
                ++index;
            }

            return { dst, count };
        }

        /** Rewinds every chunk to offset zero. Keeps the heap-allocated chunks. */
        void Reset() noexcept;

        /** Releases all heap chunks. Equivalent to destroying and re-creating the arena. */
        void ReleaseChunks() noexcept;

        /** Total bytes currently committed (sum of chunk sizes). */
        [[nodiscard]] Count GetCommittedSize() const noexcept;

        /** Total bytes currently allocated to users (sum of chunk write offsets). */
        [[nodiscard]] Count GetUsedSize() const noexcept;

        /** Number of heap chunks currently held. */
        [[nodiscard]] Count GetChunkCount() const noexcept
        {
            return m_Chunks.GetItemCount();
        }

      private:
        struct Chunk final
        {
            Byte *Data = nullptr;
            Count Size = 0;
            Count Offset = 0;
        };

        /** Allocates a new chunk of at least @p minSize bytes and appends it. */
        void AddChunk( const Count minSize );

      private:
        primitive::Vector< Chunk > m_Chunks;
        Index m_CurrentChunkIndex = s_InvalidIndex;
        Count m_InitialChunkSize;
    };
}
