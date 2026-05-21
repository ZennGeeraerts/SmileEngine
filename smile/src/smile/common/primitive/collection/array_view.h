/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::primitive
{
    template < typename ItemType >
    class ArrayView final
    {
      public:
        using Item = ItemType;

        ArrayView() = default;
        ArrayView( Item *pItems, const Count itemCount ) noexcept : m_pItems{ pItems }, m_ItemCount{ itemCount }
        {
        }

        template < Count Size >
        explicit ArrayView( Item ( &pItems )[Size] ) noexcept : ArrayView{ pItems, Size }
        {
        }

        template < typename OtherItemType,
            typename = std::enable_if< std::is_same_v< std::remove_const_t< Item >, OtherItemType > &&
                                       std::is_const_v< Item > > >
        ArrayView( const ArrayView< OtherItemType > &other ) noexcept : ArrayView{ other.m_pItems, other.m_ItemCount }
        {
        }

        ~ArrayView() = default;

        ArrayView &operator=( const ArrayView & ) = default;

        Item &operator[]( const Index index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        const Item &operator[]( const Index index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        Item *GetData() noexcept
        {
            return m_pItems;
        }

        const Item *GetData() const noexcept
        {
            return m_pItems;
        }

        Count GetItemCount() const noexcept
        {
            return m_ItemCount;
        }

        bool IsEmpty() const noexcept
        {
            return m_ItemCount == 0;
        }

        [[nodiscard]] bool IsValidIndex( const Index index ) const noexcept
        {
            return index < m_ItemCount;
        }

      public:
        Item *m_pItems = nullptr;
        Count m_ItemCount = 0;
    };

    template < typename ItemType >
    ItemType *begin( ArrayView< ItemType > &view ) noexcept
    {
        return view.GetData();
    }

    template < typename ItemType >
    ItemType *end( ArrayView< ItemType > &view ) noexcept
    {
        return view.GetData() + view.GetItemCount();
    }

    template < typename ItemType >
    const ItemType *begin( const ArrayView< ItemType > &view ) noexcept
    {
        return view.GetData();
    }

    template < typename ItemType >
    const ItemType *end( const ArrayView< ItemType > &view ) noexcept
    {
        return view.GetData() + view.GetItemCount();
    }

    template < typename ItemType >
    Count GetArrayItemCount( const ArrayView< ItemType > &view ) noexcept
    {
        return view.GetItemCount();
    }
}