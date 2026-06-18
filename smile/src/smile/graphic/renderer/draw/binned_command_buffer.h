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
 * @file        binned_command_buffer.h
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       BinnedCommandBuffer - Add, Sort, and Clear implementations.
 *
 *              BinnedCommandBuffer groups submissions by SortKey into DrawBins. Entities
 *              sharing the same sort key are automatically batched into one bin. All
 *              mesh, material, and pipeline data is resolved per entity from the
 *              RenderWorld ECS at execute time — the buffer stores only sort keys and
 *              entity handles.
 *
 *              After all submissions, call Sort() to order the bins ascending by SortKey
 *              (opaque: state-sort first, nearest-first within state). The render pass
 *              execute lambda then iterates GetBins() in sorted order.
 *
 *              Clear() resets all bins and the sorted cache without deallocating storage,
 *              making it cheap to call every frame.
 *
 *              Thread safety: not thread-safe. Populate from a single thread.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "draw_bin.h"
#include "smile/graphic/renderer/mesh/mesh_source.h"

namespace smile::graphic
{
    class BinnedCommandBuffer final
    {
      public:
        BinnedCommandBuffer() = default;
        ~BinnedCommandBuffer() = default;

        BinnedCommandBuffer( const BinnedCommandBuffer & ) = delete;
        BinnedCommandBuffer &operator=( const BinnedCommandBuffer & ) = delete;

        BinnedCommandBuffer( BinnedCommandBuffer && ) = default;
        BinnedCommandBuffer &operator=( BinnedCommandBuffer && ) = default;

        /**
         * Adds @p entity to the bin identified by @p key.
         * The bin is created on first insertion; subsequent calls with the same key
         * append the entity to the same bin.
         */
        void Add( const BinKey binKey, const SortKey sortKey, const smile::ecs::EntityHandle entity );

        /**
         * Rebuilds the sorted bin list and orders it ascending by SortKey.
         * Idempotent if the buffer has not been modified since the last Sort() call.
         */
        void Sort();

        /**
         * Returns the sorted bin list. Sort() must be called before GetBins() if any
         * Add() calls were made since the last Sort().
         */
        [[nodiscard]] const primitive::Vector< DrawBin * > &GetSorted() const noexcept
        {
            return m_SortedBins;
        }

        /** Number of bins currently in the buffer. */
        [[nodiscard]] Count GetBinCount() const noexcept
        {
            return m_Bins.GetItemCount();
        }

        /** True if the buffer has no bins. */
        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return m_Bins.IsEmpty();
        }

        /**
         * Removes all bins and clears the sorted cache.
         * Keeps the underlying storage allocated for reuse next frame.
         */
        void Clear() noexcept;

      private:
        primitive::HashMap< BinKey, DrawBin > m_Bins;
        primitive::Vector< DrawBin * > m_SortedBins;
        bool m_Sorted = false;
    };
}
