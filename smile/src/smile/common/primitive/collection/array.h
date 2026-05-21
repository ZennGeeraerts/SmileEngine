/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array_iterator.h"
#include "array_view.h"
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

        Array( const Array &other ) noexcept
        {
            Set( other );
        }

        ~Array() = default;

        Array &operator=( const Array &other ) noexcept
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

        bool operator==( const Array &other ) const noexcept
        {
            return std::equal( std::begin( *this ), std::end( *this ), std::begin( other ) );
        }

        bool operator!=( const Array &other ) const noexcept
        {
            return !( *this == other );
        }

        operator ArrayView< Item >() noexcept
        {
            return ArrayView{ m_Items, ItemCount };
        }

        operator ArrayView< const Item >() const noexcept
        {
            return ArrayView{ m_Items, ItemCount };
        }

        [[nodiscard]] virtual bool IsValidIndex( const Index index ) const noexcept
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

        const Item *GetData() const noexcept
        {
            return m_Items;
        }

        Item *GetData() noexcept
        {
            return m_Items;
        }

        static constexpr Count GetItemCount() noexcept
        {
            return ItemCount;
        }

        void Set( const Array< ItemType, ItemCount > &other ) noexcept
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

        Iterator begin() noexcept
        {
            return { this, 0 };
        }

        ConstIterator begin() const noexcept
        {
            return { this, 0 };
        }

        Iterator end() noexcept
        {
            return { this, ItemCount };
        }

        ConstIterator end() const noexcept
        {
            return { this, ItemCount };
        }

        ArrayView< Item > AsView( const Count itemCount = s_InvalidCount );
        ArrayView< const Item > AsView( const Count itemCount = s_InvalidCount ) const;

      private:
        Item m_Items[ItemCount];
    };

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::Iterator begin( Array< ItemType, ItemCount > &array ) noexcept
    {
        return array.begin();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator begin( const Array< ItemType, ItemCount > &array ) noexcept
    {
        return array.begin();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::Iterator end( Array< ItemType, ItemCount > &array ) noexcept
    {
        return array.end();
    }

    template < typename ItemType, Count ItemCount >
    typename Array< ItemType, ItemCount >::ConstIterator end( const Array< ItemType, ItemCount > &array ) noexcept
    {
        return array.end();
    }

    template < typename ItemType, Count ItemCount >
    inline constexpr Count GetArrayItemCount( const Array< ItemType, ItemCount > & ) noexcept
    {
        return ItemCount;
    }

    template < typename ItemType, Count ItemCount >
    ArrayView< ItemType > Array< ItemType, ItemCount >::AsView( const Count itemCount )
    {
        auto count = itemCount == s_InvalidCount ? GetItemCount() : itemCount;

        SM_ASSERT( count <= GetItemCount() );

        return { GetData(), count };
    }

    template < typename ItemType, Count ItemCount >
    ArrayView< const ItemType > Array< ItemType, ItemCount >::AsView( const Count itemCount ) const
    {
        auto count = itemCount == s_InvalidCount ? GetItemCount() : itemCount;

        SM_ASSERT( count <= GetItemCount() );

        return { GetData(), count };
    }
}