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
 * @file        render_resource_table.h
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Ref-counted slot map keyed by RenderHandle.
 *
 *              Slot allocation, generation bumping, and stale-handle detection are
 *              delegated to the existing `primitive::HandleManager`. This table
 *              layers ref-counting and dense payload storage on top: items live in
 *              a parallel vector indexed by handle slot, and a parallel refcount
 *              vector controls when the underlying slot is destroyed.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/handle_manager.h"
#include "smile/graphic/renderer/scene/render_ids.h"

namespace smile::graphic
{
    /**
     * Slot-map storing @p ItemType keyed by `RenderHandle`. Items must be
     * trivially relocatable; a freed slot's storage is overwritten on next insert.
     */
    template < typename ItemType >
    class RenderResourceTable final
    {
      public:
        using HandleType = RenderHandle;

        RenderResourceTable() = default;
        ~RenderResourceTable() = default;

        RenderResourceTable( const RenderResourceTable & ) = delete;
        RenderResourceTable &operator=( const RenderResourceTable & ) = delete;

        /** Inserts @p item and returns its handle. Initial ref-count = 1. */
        [[nodiscard]] HandleType Insert( ItemType item )
        {
            const HandleType handle = m_Handles.CreateHandle();
            const Index slotIndex = handle.GetIndex();

            if ( slotIndex >= m_Items.GetItemCount() )
            {
                m_Items.PushBack( std::move( item ) );
                m_RefCounts.PushBack( 1u );
            }
            else
            {
                m_Items[slotIndex] = std::move( item );
                m_RefCounts[slotIndex] = 1u;
            }

            return handle;
        }

        /**
         * Returns a pointer to the item if @p handle is alive and its generation matches,
         * otherwise nullptr.
         */
        [[nodiscard]] ItemType *TryGet( const HandleType handle ) noexcept
        {
            return IsAlive( handle ) ? &m_Items[handle.GetIndex()] : nullptr;
        }

        [[nodiscard]] const ItemType *TryGet( const HandleType handle ) const noexcept
        {
            return IsAlive( handle ) ? &m_Items[handle.GetIndex()] : nullptr;
        }

        /** Returns the item by reference. Asserts the handle is alive. */
        [[nodiscard]] ItemType &Get( const HandleType handle ) noexcept
        {
            SM_ASSERT( IsAlive( handle ) );
            return m_Items[handle.GetIndex()];
        }

        [[nodiscard]] const ItemType &Get( const HandleType handle ) const noexcept
        {
            SM_ASSERT( IsAlive( handle ) );
            return m_Items[handle.GetIndex()];
        }

        /** Increments the ref-count for the slot. Asserts the handle is alive. */
        void Acquire( const HandleType handle ) noexcept
        {
            SM_ASSERT( IsAlive( handle ) );
            ++m_RefCounts[handle.GetIndex()];
        }

        /**
         * Decrements the ref-count. When it reaches zero the slot is destroyed via the
         * HandleManager (which bumps its generation, making prior handles stale).
         * @return  true if the slot was freed by this call.
         */
        bool Release( const HandleType handle ) noexcept
        {
            if ( !IsAlive( handle ) )
                return false;

            Count &refCount = m_RefCounts[handle.GetIndex()];
            SM_ASSERT( refCount > 0 );
            --refCount;

            if ( refCount == 0 )
            {
                m_Handles.DestroyHandle( handle );
                return true;
            }

            return false;
        }

        /** True if @p handle refers to a live slot with the matching generation. */
        [[nodiscard]] bool IsAlive( const HandleType handle ) const noexcept
        {
            return m_Handles.IsHandleActive( handle );
        }

        /** Current ref-count for the slot, or 0 if the handle is stale. */
        [[nodiscard]] Count GetRefCount( const HandleType handle ) const noexcept
        {
            return IsAlive( handle ) ? m_RefCounts[handle.GetIndex()] : 0u;
        }

        /** Number of currently-alive slots. O(slots). */
        [[nodiscard]] Count GetAliveCount() const noexcept
        {
            Count count = 0;
            for ( [[maybe_unused]] const auto &h : m_Handles )
                ++count;
            return count;
        }

        /** Total capacity (alive + free). */
        [[nodiscard]] Count GetCapacity() const noexcept
        {
            return m_Handles.GetHandleCount();
        }

        /** Resets every slot. */
        void Clear() noexcept
        {
            m_Handles = primitive::HandleManager< Uint32, 24, 8 >{};
            m_Items.Clear();
            m_RefCounts.Clear();
        }

      private:
        primitive::HandleManager< Uint32, 24, 8 > m_Handles;
        primitive::Vector< ItemType > m_Items;
        primitive::Vector< Count > m_RefCounts;
    };
}
