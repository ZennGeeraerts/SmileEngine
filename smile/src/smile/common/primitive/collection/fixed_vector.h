/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array.h"

namespace smile::primitive
{
    template < typename ItemType, Count MaxItemCount >
    class FixedVector final : public Array< ItemType, MaxItemCount >
    {
      public:
        using Base = typename Array< ItemType, MaxItemCount >;
        using Item = ItemType;
        using Iterator = typename ArrayIterator< FixedVector< Item, MaxItemCount >, false >;
        using ConstIterator = typename ArrayIterator< FixedVector< Item, MaxItemCount >, true >;

        FixedVector() : Base{}, m_CurrentItemCount{ 0 }
        {
        }

        FixedVector( const Count itemCount ) : Base{}, m_CurrentItemCount{ itemCount }
        {
            SM_ASSERT( itemCount <= MaxItemCount );
        }

        FixedVector( std::initializer_list< Item > items ) : m_CurrentItemCount{ 0 }
        {
            for ( const auto &item : items )
            {
                PushBack( item );
            }
        }

        using Array< Item, MaxItemCount >::GetData;

        bool IsValidIndex( const Index index ) const override
        {
            return index < m_CurrentItemCount;
        }

        Count GetCurrentItemCount() const
        {
            return m_CurrentItemCount;
        }

        bool IsEmpty() const
        {
            return m_CurrentItemCount == 0;
        }

        void PushBack( const Item &item ) noexcept
        {
            SM_ASSERT( m_CurrentItemCount < MaxItemCount );

            *( GetData() + m_CurrentItemCount ) = item;
            ++m_CurrentItemCount;
        }

        void PushBack( Item &&item ) noexcept
        {
            SM_ASSERT( m_CurrentItemCount < MaxItemCount );

            *( GetData() + m_CurrentItemCount ) = std::move( item );
            ++m_CurrentItemCount;
        }

        void PopBack() noexcept
        {
            SM_ASSERT( m_CurrentItemCount > 0 );

            --m_CurrentItemCount;
        }

        void Resize( const Count newItemCount ) noexcept
        {
            SM_ASSERT( newItemCount <= MaxItemCount );

            if ( m_CurrentItemCount > newItemCount )
            {
                for ( Count i = newItemCount; i < m_CurrentItemCount; ++i )
                    *( GetData() + i ) = Item{};
            }
            else
            {
                for ( Count i = m_CurrentItemCount; i < newItemCount; ++i )
                    *( GetData() + i ) = Item{};
            }

            m_CurrentItemCount = newItemCount;
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
            return { this, m_CurrentItemCount };
        }

        inline ConstIterator end() const
        {
            return { this, m_CurrentItemCount };
        }

      private:
        Count m_CurrentItemCount;
    };
}