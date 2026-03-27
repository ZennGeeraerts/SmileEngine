/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/assert.h"

#include <utility>

namespace smile::primitive
{
    template < typename Collection, bool IsConst >
    class ArrayIterator final
    {
      public:
        using ItemType = typename Collection::Item;
        using CollectionType = typename std::conditional_t< IsConst, const Collection, Collection >;
        friend class ArrayIterator< Collection, !IsConst >;

        using value_type = ItemType;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = Index;
        using reference = ItemType &;
        using pointer = ItemType *;

        ArrayIterator() : m_pCollection{ nullptr }, m_Index{ 0 }
        {
        }

        ArrayIterator( const ArrayIterator & ) = default;

        ArrayIterator &operator=( const ArrayIterator & ) = default;

        template < bool OtherIsConst >
        bool operator==( const ArrayIterator< Collection, OtherIsConst > &other ) const
        {
            return ( m_pCollection == other.m_pCollection ) && ( m_Index == other.m_Index );
        }

        bool operator!=( const ArrayIterator &other ) const
        {
            return !( *this == other );
        }

        bool operator<( const ArrayIterator &other ) const
        {
            return m_Index < other.m_Index;
        }

        bool operator<=( const ArrayIterator &other ) const
        {
            return m_Index <= other.m_Index;
        }

        bool operator>( const ArrayIterator &other ) const
        {
            return m_Index > other.m_Index;
        }

        bool operator>=( const ArrayIterator &other ) const
        {
            return m_Index >= other.m_Index;
        }

        ArrayIterator &operator++()
        {
            SM_ASSERT( m_Index < m_pCollection->GetItemCount() );

            ++m_Index;

            return *this;
        }

        ArrayIterator operator++( int )
        {
            SM_ASSERT( m_Index < m_pCollection->GetItemCount() );

            auto temp = *this;
            ++m_Index;

            return std::move( temp );
        }

        ArrayIterator &operator--()
        {
            SM_ASSERT( m_Index > 0 );

            --m_Index;

            return *this;
        }

        ArrayIterator operator--( int )
        {
            SM_ASSERT( m_Index > 0 );

            auto temp = *this;
            --m_Index;

            return std::move( temp );
        }

        ArrayIterator operator+( const Index offset ) const
        {
            return { m_pCollection, m_Index + offset };
        }

        ArrayIterator operator+=( const Index offset )
        {
            auto temp = *this;

            m_Index += offset;

            return std::move( temp );
        }

        Index operator-( const ArrayIterator &other ) const
        {
            SM_ASSERT( m_pCollection == other.m_pCollection );

            return m_Index - other.m_Index;
        }

        ArrayIterator operator-( const Index offset ) const
        {
            SM_ASSERT( m_Index >= offset );

            return { m_pCollection, m_Index - offset };
        }

        typename std::conditional_t< IsConst, const ItemType, ItemType > &operator*() const
        {
            SM_ASSERT( m_pCollection->IsValidIndex( m_Index ) );

            return ( *m_pCollection )[m_Index];
        }

        typename std::conditional_t< IsConst, const ItemType, ItemType > &operator[]( const Index offset ) const
        {
            SM_ASSERT( m_pCollection->IsValidIndex( m_Index + offset ) );

            return ( *m_pCollection )[m_Index + offset];
        }

        typename std::conditional_t< IsConst, const ItemType, ItemType > *operator->() const
        {
            SM_ASSERT( m_pCollection->IsValidIndex( m_Index ) );

            return &( ( *m_pCollection )[m_Index] );
        }

        Index GetIndex() const
        {
            return m_Index;
        }

      private:
        friend CollectionType;

        ArrayIterator( CollectionType *pCollection, const Index index ) : m_pCollection{ pCollection }, m_Index{ index }
        {
        }

      private:
        CollectionType *m_pCollection;
        Index m_Index;
    };
}