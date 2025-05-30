/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array_iterator.h"
#include "smile/common/memory/memory.h"

#include <initializer_list>

namespace smile::primitive
{
    template < typename ItemType, int ItemCount >
    class Array
    {
      public:
        using Item = ItemType;
        using Iterator = typename ArrayIterator< Array< Item, ItemCount >, false >;
        using ConstIterator = typename ArrayIterator< Array< Item, ItemCount >, true >;

        Array() = default;

        Array( std::initializer_list< Item > items )
        {
            SM_ASSERT( items.size() == ItemCount );
            int index = 0;

            for ( const Item &item : items )
            {
                m_Items[index] = item;
                ++index;
            }
        }

        Array( const Array &other )
        {
            Set( other );
        }

        ~Array() = default;

        Array &operator=( const Array &other )
        {
            Set( other );

            return *this;
        }

        Item &operator[]( const int index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        const Item &operator[]( const int index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        bool IsValidIndex( const int index ) const
        {
            return ( 0 <= index ) && ( index < ItemCount );
        }

        const Item &GetItemAtIndex( const int index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        void SetItemAtIndex( const Item &item, const int index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            m_Items[index] = item;
        }

        const Item *GetData() const
        {
            return m_Items;
        }

        Item *GetData()
        {
            return m_Items;
        }

        int GetItemCount() const
        {
            return ItemCount;
        }

        void Set( const Array< ItemType, ItemCount > &other )
        {
            if constexpr ( memory::IsRawType< Item >() )
            {
                std::memcpy( m_Items, other.m_Items, ItemCount * sizeof( Item ) );
            }
            else
            {
                memory::CopyArrayItems( m_Items, ItemCount, other.m_Items );
            }
        }

        inline Iterator begin()
        {
            return { this, 0 };
        }

        inline ConstIterator begin() const
        {
            return { this, 0 };
        }

        inline Iterator end()
        {
            return { this, ItemCount };
        }

        inline ConstIterator end() const
        {
            return { this, ItemCount };
        }

      private:
        Item m_Items[ItemCount];
    };

    template < typename ItemType, int ItemCount >
    typename Array< ItemType, ItemCount >::Iterator begin( Array< ItemType, ItemCount > &array )
    {
        return array.begin();
    }

    template < typename ItemType, int ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator begin( const Array< ItemType, ItemCount > &array )
    {
        return array.begin();
    }

    template < typename ItemType, int ItemCount >
    typename Array< ItemType, ItemCount >::Iterator end( Array< ItemType, ItemCount > &array )
    {
        return array.end();
    }

    template < typename ItemType, int ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator end( const Array< ItemType, ItemCount > &array )
    {
        return array.end();
    }

    template < typename ItemType, int ItemCount >
    inline int GetArrayItemCount( const Array< ItemType, ItemCount > & )
    {
        return ItemCount;
    }
}