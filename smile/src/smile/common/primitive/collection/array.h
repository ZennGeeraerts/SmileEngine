/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array_iterator.h"
#include "smile/common/memory/memory.h"
#include "smile/common/foundation/numeric_cast.h"

#include <initializer_list>

namespace smile::primitive
{
    template < typename ItemType, Count ItemCount >
    class Array
    {
      public:
        using Item = ItemType;
        using Iterator = typename ArrayIterator< Array< Item, ItemCount >, false >;
        using ConstIterator = typename ArrayIterator< Array< Item, ItemCount >, true >;

        Array() = default;

        Array( std::initializer_list< Item > items )
        {
            SM_ASSERT( foundation::NumericCast< Count >( items.size() ) == ItemCount );

            Index index = 0;
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

        Item &operator[]( const Index index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        const Item &operator[]( const Index index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        bool operator==( const Array &other ) const
        {
            return std::equal( primitive::begin( *this ), primitive::end( *this ), primitive::begin( other ) );
        }

        bool operator!=( const Array &other ) const
        {
            return !( *this == other );
        }

        virtual bool IsValidIndex( const Index index ) const
        {
            return index < ItemCount;
        }

        const Item &GetItemAtIndex( const Index index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        void SetItemAtIndex( const Item &item, const Index index )
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

        Count GetItemCount() const
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

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::Iterator begin( Array< ItemType, ItemCount > &array )
    {
        return array.begin();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator begin( const Array< ItemType, ItemCount > &array )
    {
        return array.begin();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::Iterator end( Array< ItemType, ItemCount > &array )
    {
        return array.end();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator end( const Array< ItemType, ItemCount > &array )
    {
        return array.end();
    }

    template < typename ItemType, Count ItemCount >
    inline Count GetArrayItemCount( const Array< ItemType, ItemCount > & )
    {
        return ItemCount;
    }
}