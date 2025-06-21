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
        ArrayView( Item *pItems, const Count itemCount ) : m_pItems{ pItems }, m_ItemCount{ itemCount }
        {
        }

        template < Count Size >
        ArrayView( Item ( &pItems )[Size] ) : ArrayView{ pItems, Size }
        {
        }

        template < typename OtherItemType,
            typename = std::enable_if< std::is_same_v< std::remove_const_t< Item >, OtherItemType > &&
                                       std::is_const_v< Item > > >
        ArrayView( const ArrayView< OtherItemType > &other ) : ArrayView{ other.m_pItems, other.m_ItemCount }
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

        Item *GetData()
        {
            return m_pItems;
        }

        const Item *GetData() const
        {
            return m_pItems;
        }

        Count GetItemCount() const
        {
            return m_ItemCount;
        }

        bool IsEmpty() const
        {
            return m_ItemCount == 0;
        }

        bool IsValidIndex( const Index index ) const
        {
            return index < m_ItemCount;
        }

      public:
        Item *m_pItems = nullptr;
        Count m_ItemCount = 0;
    };

    template < typename ItemType >
    ItemType *begin( ArrayView< ItemType > &view )
    {
        return view.GetData();
    }

    template < typename ItemType >
    ItemType *end( ArrayView< ItemType > &view )
    {
        return view.GetData() + view.GetItemCount();
    }

    template < typename ItemType >
    const ItemType *begin( const ArrayView< ItemType > &view )
    {
        return view.GetData();
    }

    template < typename ItemType >
    const ItemType *end( const ArrayView< ItemType > &view )
    {
        return view.GetData() + view.GetItemCount();
    }

    template < typename ItemType >
    inline Count GetArrayItemCount( const ArrayView< ItemType > &view )
    {
        return view.GetItemCount();
    }
}