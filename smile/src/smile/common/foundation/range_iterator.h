/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "compiled.h"

namespace smile::foundation
{
    template < bool IsForward >
    class RangeIterator final
    {
      public:
        RangeIterator( const Index index ) : m_Index{ index }
        {
        }

        bool operator==( const RangeIterator &other ) const
        {
            return m_Index == other.m_Index;
        }

        bool operator!=( const RangeIterator &other ) const
        {
            return m_Index != other.m_Index;
        }

        RangeIterator &operator++()
        {
            if ( IsForward )
            {
                ++m_Index;
            }
            else
            {
                --m_Index;
            }

            return *this;
        }

        Index operator*() const
        {
            return m_Index;
        }

      private:
        Index m_Index;
    };

    template < bool IsForward >
    class RangeHolder final
    {
      public:
        RangeHolder( const Index firstIndex, const Count count ) : m_FirstIndex{ firstIndex }, m_Count{ count }
        {
        }

        friend RangeIterator< IsForward > begin( const RangeHolder &holder )
        {
            return { holder.m_FirstIndex + ( IsForward ? 0 : ( holder.m_Count - 1 ) ) };
        }

        friend RangeIterator< IsForward > end( const RangeHolder &holder )
        {
            return { holder.m_FirstIndex + ( IsForward ? holder.m_Count : -1 ) };
        }

      private:
        Index m_FirstIndex;
        Count m_Count;
    };

    template < typename ItemType, int ItemCount >
    inline constexpr Count GetArrayItemCount( const ItemType ( & )[ItemCount] )
    {
        return ItemCount;
    }

    inline RangeHolder< true > GetCountIterator( const Count count )
    {
        return { 0, count };
    }

    template < typename CollectionType >
    inline RangeHolder< true > GetCountIterator( const CollectionType &collection )
    {
        return { 0, GetArrayItemCount( collection ) };
    }

    inline RangeHolder< false > GetReverseCountIterator( const Count count )
    {
        return { 0, count };
    }

    template < typename CollectionType >
    inline RangeHolder< false > GetReverseCountIterator( const CollectionType &collection )
    {
        return { 0, GetArrayItemCount( collection ) };
    }

    inline RangeHolder< true > GetRangeIterator( const Index firstIndex, const Index lastIndex )
    {
        SM_ASSERT( firstIndex <= lastIndex );

        return { firstIndex, lastIndex - firstIndex };
    }
}