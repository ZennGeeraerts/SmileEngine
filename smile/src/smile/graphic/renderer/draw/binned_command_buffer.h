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
#include "smile/graphic/renderer/mesh/mesh_source.h"
#include "sort_key.h"

#include <algorithm>

namespace smile::graphic
{
    template < typename BinnedItem >
    struct DrawBin final
    {
        using BinKey = typename BinnedItem::BinKey;

        BinKey Key;
        primitive::Vector< BinnedItem > Items;

        void Sort() noexcept
        {
            std::sort( Items.begin(),
                Items.end(),
                []( const BinnedItem &a, const BinnedItem &b ) { return a.GetSortKey() < b.GetSortKey(); } );
        }

        Count GetInstanceCount() const noexcept
        {
            return Items.GetItemCount();
        }
    };

    template < typename BinnedItem >
    class BinnedCommandBuffer final
    {
      public:
        using BinKey = typename BinnedItem::BinKey;

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
        void Add( const BinKey binKey, const BinnedItem &item )
        {
            auto [it, inserted] = m_Bins.TryCreateItemAtKey( binKey );
            DrawBin< BinnedItem > &bin = it.GetItem();

            SM_ASSERT( inserted || bin.Key == binKey );

            bin.Items.PushBack( item );
            m_Sorted = false;
        }

        /**
         * Rebuilds the sorted bin list and orders it ascending by SortKey.
         * Idempotent if the buffer has not been modified since the last Sort() call.
         */
        void Sort()
        {
            if ( m_Sorted )
                return;

            m_SortedBins.Clear();

            for ( auto it = m_Bins.begin(); it != m_Bins.end(); ++it )
                m_SortedBins.PushBack( &it.GetItem() );

            std::sort( m_SortedBins.begin(),
                m_SortedBins.end(),
                []( const DrawBin< BinnedItem > *a, const DrawBin< BinnedItem > *b ) noexcept
                { return a->Key < b->Key; } );

            for ( DrawBin< BinnedItem > *bin : m_SortedBins )
                bin->Sort();

            m_Sorted = true;
        }

        /**
         * Returns the sorted bin list. Sort() must be called before GetBins() if any
         * Add() calls were made since the last Sort().
         */
        [[nodiscard]] const primitive::Vector< DrawBin< BinnedItem > * > &GetSorted() const noexcept
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
        void Clear() noexcept
        {
            m_Bins.Clear();
            m_SortedBins.Clear();
            m_Sorted = false;
        }

      private:
        primitive::HashMap< BinKey, DrawBin< BinnedItem > > m_Bins;
        primitive::Vector< DrawBin< BinnedItem > * > m_SortedBins;
        bool m_Sorted = false;
    };

    template < typename BinnedItem >
    class ViewBinnedCommandBuffers final
    {
      public:
        ViewBinnedCommandBuffers() = default;
        ~ViewBinnedCommandBuffers() = default;

        ViewBinnedCommandBuffers( const ViewBinnedCommandBuffers & ) = delete;
        ViewBinnedCommandBuffers &operator=( const ViewBinnedCommandBuffers & ) = delete;

        ViewBinnedCommandBuffers( ViewBinnedCommandBuffers && ) = default;
        ViewBinnedCommandBuffers &operator=( ViewBinnedCommandBuffers && ) = default;

        BinnedCommandBuffer< BinnedItem > &operator[]( const smile::ecs::EntityHandle viewEntity )
        {
            return m_BufferPerView[viewEntity];
        }

        void Clear()
        {
            for ( const auto &[viewEntity, buffer] : m_BufferPerView )
                buffer.Clear();
        }

      private:
        primitive::HashMap< smile::ecs::EntityHandle, BinnedCommandBuffer< BinnedItem > > m_BufferPerView;
    };
}
