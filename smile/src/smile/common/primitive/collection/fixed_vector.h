/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array.h"

namespace smile::primitive
{
    template < typename ItemType, Count MaxItemCount >
    class FixedVector final : protected Array< ItemType, MaxItemCount >
    {
      public:
        using Base = typename Array< ItemType, MaxItemCount >;
        using Item = ItemType;
        using Iterator = typename ArrayIterator< FixedVector< Item, MaxItemCount >, false >;
        using ConstIterator = typename ArrayIterator< FixedVector< Item, MaxItemCount >, true >;

        FixedVector() noexcept : Base{}, m_ItemCount{ 0 }
        {
        }

        explicit FixedVector( const Count itemCount ) : Base{}, m_ItemCount{ itemCount }
        {
            SM_ASSERT( itemCount <= MaxItemCount );
        }

        FixedVector( std::initializer_list< Item > items ) : m_ItemCount{ 0 }
        {
            for ( const auto &item : items )
            {
                PushBack( item );
            }
        }

        using Array< Item, MaxItemCount >::operator[];

        bool operator==( const FixedVector &other ) const
        {
            if ( m_ItemCount != other.m_ItemCount )
                return false;

            return std::equal( std::begin( *this ), std::end( *this ), std::begin( other ) );
        }

        bool operator!=( const FixedVector &other ) const
        {
            return !( *this == other );
        }

        using Array< Item, MaxItemCount >::GetData;

        [[nodiscard]] bool IsValidIndex( const Index index ) const noexcept override
        {
            return index < m_ItemCount;
        }

        Count GetItemCount() const noexcept
        {
            return m_ItemCount;
        }

        static constexpr Count GetMaxItemCount() noexcept
        {
            return MaxItemCount;
        }

        bool IsEmpty() const noexcept
        {
            return m_ItemCount == 0;
        }

        void PushBack( const Item &item )
        {
            SM_ASSERT( m_ItemCount < MaxItemCount );

            *( GetData() + m_ItemCount ) = item;
            ++m_ItemCount;
        }

        void PushBack( Item &&item )
        {
            SM_ASSERT( m_ItemCount < MaxItemCount );

            *( GetData() + m_ItemCount ) = std::move( item );
            ++m_ItemCount;
        }

        template < typename... Arguments >
        void EmplaceBack( Arguments &&...arguments )
        {
            SM_ASSERT( m_ItemCount < MaxItemCount );

            ::new ( GetData() + m_ItemCount, memory::g_pInPlace ) Item{ std::forward< Arguments >( arguments )... };
            ++m_ItemCount;
        }

        void PopBack()
        {
            SM_ASSERT( m_ItemCount > 0 );

            --m_ItemCount;
        }

        void Resize( const Count newItemCount )
        {
            SM_ASSERT( newItemCount <= MaxItemCount );

            if ( m_ItemCount > newItemCount )
            {
                for ( Count i = newItemCount; i < m_ItemCount; ++i )
                    *( GetData() + i ) = Item{};
            }
            else
            {
                for ( Count i = m_ItemCount; i < newItemCount; ++i )
                    *( GetData() + i ) = Item{};
            }

            m_ItemCount = newItemCount;
        }

        void Clear() noexcept
        {
            Resize( 0 );
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
            return { this, m_ItemCount };
        }

        ConstIterator end() const noexcept
        {
            return { this, m_ItemCount };
        }

      private:
        Count m_ItemCount;
    };
}