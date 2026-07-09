/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        slot_map.h
 * @author      Zenn Geeraerts
 * @created     9 July 2026
 * @brief       A slot map is a data structure that allows for storage and retrieval of items using handles.
 *              It provides a way to manage a fixed-size collection of items while allowing for dynamic insertion and
 *              removal of items. Each item is associated with a unique handle that can be used to access the item, and
 *              the slot map ensures that handles remain valid even after items are removed and slots are reused.
 */
#pragma once

#include "smile/common/primitive/handle_manager.h"
#include "array.h"

namespace smile::primitive
{
    template < typename Type,
        Count ItemCount,
        typename IndexType = Uint32,
        Uint32 IndexBits = 16,
        Uint32 GenerationBits = 16,
        typename Tag = void >
    class SlotMap final
    {
      public:
        using HandleManagerType = HandleManager< IndexType, IndexBits, GenerationBits, Tag >;
        using HandleType = typename HandleManagerType::HandleType;

        SlotMap() = default;
        ~SlotMap() = default;

        SlotMap( const SlotMap & ) = delete;
        SlotMap( SlotMap && ) = delete;
        SlotMap &operator=( const SlotMap & ) = delete;
        SlotMap &operator=( SlotMap && ) = delete;

        HandleType Insert( const Type &value ) noexcept
        {
            const HandleType handle = m_HandleManager.CreateHandle();
            m_Values[handle.GetIndex()] = value;
            return handle;
        }

        HandleType Insert( Type &&value ) noexcept
        {
            const HandleType handle = m_HandleManager.CreateHandle();
            m_Values[handle.GetIndex()] = std::move( value );
            return handle;
        }

        bool HasItemAtSlot( HandleType handle ) const noexcept
        {
            return m_HandleManager.IsHandleActive( handle );
        }

        const Type &GetItemAtSlot( HandleType handle ) const noexcept
        {
            SM_ASSERT( HasItemAtSlot( handle ) );

            return m_Values[handle.GetIndex()];
        }

        Type &GetItemAtSlot( HandleType handle ) noexcept
        {
            SM_ASSERT( HasItemAtSlot( handle ) );

            return m_Values[handle.GetIndex()];
        }

        void EraseItemAtSlot( HandleType handle ) noexcept
        {
            SM_ASSERT( HasItemAtSlot( handle ) );

            m_HandleManager.DestroyHandle( handle );
        }

      private:
        Array< Type, ItemCount > m_Values;
        HandleManagerType m_HandleManager;
    };
}