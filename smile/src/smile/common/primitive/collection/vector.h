/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "array_iterator.h"
#include "smile/common/foundation/numeric_cast.h"
#include "smile/common/foundation/range_iterator.h"
#include "smile/common/memory/memory.h"

#include <initializer_list>

namespace smile::primitive
{
    template < typename ItemType >
    class Vector
    {
      public:
        using Item = ItemType;
        using Iterator = typename ArrayIterator< Vector< Item >, false >;
        using ConstIterator = typename ArrayIterator< Vector< Item >, true >;

        Vector() : m_pItems{ nullptr }, m_ItemCount{ 0 }
        {
        }

        Vector( std::initializer_list< Item > items ) : m_pItems{ nullptr }, m_ItemCount{ 0 }
        {
            auto itemCount = foundation::NumericCast< Count >( items.size() );
            Reserve( itemCount );
            m_ItemCount = itemCount;

            Index index = 0;
            for ( const Item &item : items )
            {
                new ( m_pItems + index, memory::g_pInPlace ) Item{ item };
                ++index;
            }
        }

        Vector( const Item *pOther, const Count itemCount ) : m_pItems{ nullptr }, m_ItemCount{ 0 }
        {
            Reserve( itemCount );
            memory::ConstructCopiedArrayItems( m_pItems, itemCount, pOther );
            m_ItemCount = itemCount;
        }

        Vector( const Vector &other ) : m_pItems{ nullptr }, m_ItemCount{ 0 }
        {
            Reserve( other.m_ItemCount );
            memory::ConstructCopiedArrayItems( m_pItems, other.m_ItemCount, other.m_pItems );
            m_ItemCount = other.m_ItemCount;
        }

        Vector( Vector &&other ) : m_pItems{ other.m_pItems }, m_ItemCount{ other.m_ItemCount }
        {
            other.m_pItems = nullptr;
            other.m_ItemCount = 0;
        }

        ~Vector()
        {
            Clear();
        }

        Vector &operator=( const Vector &other )
        {
            SM_ASSERT( this != &other );

            SetItemCount( 0 );
            Reserve( other.m_ItemCount );
            memory::ConstructCopiedArrayItems( m_pItems, other.m_ItemCount, other.m_pItems );
            m_ItemCount = other.m_ItemCount;

            return *this;
        }

        Vector &operator=( Vector &&other )
        {
            Clear();
            m_pItems = other.m_pItems;
            m_ItemCount = other.m_ItemCount;
            other.m_pItems = nullptr;
            other.m_ItemCount = 0;

            return *this;
        }

        inline Item &operator[]( const Index index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        inline const Item &operator[]( const Index index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        inline Count GetItemCount() const
        {
            return m_ItemCount;
        }

        inline Item *GetData()
        {
            return m_pItems;
        }

        inline const Item *GetData() const
        {
            return m_pItems;
        }

        inline bool IsValidIndex( const Index index ) const
        {
            return index >= 0 && index < m_ItemCount;
        }

        inline Item &GetItemAtIndex( const Index index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        inline const Item &GetItemAtIndex( const Index index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_pItems[index];
        }

        inline void SetItemAtIndex( const Item &item, const Index index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            m_pItems[index] = item;
        }

        inline Item &GetLastItem()
        {
            SM_ASSERT( !IsEmpty() );

            return m_pItems[m_ItemCount - 1];
        }

        inline const Item &GetLastItem() const
        {
            SM_ASSERT( !IsEmpty() );

            return m_pItems[m_ItemCount - 1];
        }

        inline Item &GetFirstItem()
        {
            SM_ASSERT( !IsEmpty() );

            return m_pItems[0];
        }

        inline const Item &GetFirstItem() const
        {
            SM_ASSERT( !IsEmpty() );

            return m_pItems[0];
        }

        inline Iterator begin()
        {
            return Iterator{ this, 0, m_pItems };
        }

        inline ConstIterator begin() const
        {
            return ConstIterator{ this, 0, m_pItems };
        }

        inline Iterator end()
        {
            return Iterator{ this, m_ItemCount, m_pItems };
        }

        inline ConstIterator end() const
        {
            return ConstIterator{ this, m_ItemCount, m_pItems };
        }

        inline bool IsEmpty() const
        {
            return m_ItemCount == 0;
        }

        inline Index GetLastIndex() const
        {
            return m_ItemCount - 1;
        }

        inline Count GetReserved() const
        {
            return m_pItems ? memory::GetSize( m_pItems ) / sizeof( Item ) : 0;
        }

        Index GetItemIndex( const Item &item ) const
        {
            for ( auto index : foundation::GetCountIterator( m_ItemCount ) )
            {
                if ( m_pItems[index] == item )
                {
                    return index;
                }
            }

            return s_InvalidIndex;
        }

        Count GetItemSize() const
        {
            return sizeof( Item );
        }

        void Clear()
        {
            if ( !m_pItems )
                return;

            memory::DestructArrayItems( m_pItems, m_ItemCount );
            memory::DeallocateByteArray( m_pItems );

            m_pItems = nullptr;
            m_ItemCount = 0;
        }

        void SetItemCount( const Count newItemCount )
        {
            if ( m_ItemCount >= newItemCount )
            {
                memory::DestructArrayItems( &m_pItems[newItemCount], m_ItemCount - newItemCount );
            }
            else
            {
                Reserve( newItemCount );
                memory::ConstructArrayItems( &m_pItems[m_ItemCount], newItemCount - m_ItemCount );
            }

            m_ItemCount = newItemCount;
        }

        void PushBack()
        {
            Reserve( m_ItemCount + 1 );
            ::new ( m_pItems + m_ItemCount, memory::g_pInPlace ) Item();
            ++m_ItemCount;
        }

        void PushBack( const Item &item )
        {
            Reserve( m_ItemCount + 1 );
            ::new ( m_pItems + m_ItemCount, memory::g_pInPlace ) Item( item );
            ++m_ItemCount;
        }

        void PushBack( Item &&item )
        {
            Reserve( m_ItemCount + 1 );
            ::new ( m_pItems + m_ItemCount, memory::g_pInPlace ) Item( std::move( item ) );
            ++m_ItemCount;
        }

        void PushBack( const Vector< Item > &items )
        {
            Reserve( m_ItemCount + items.GetItemCount() );

            Count newItemCount = m_ItemCount;

            for ( const Item &item : items )
            {
                ::new ( m_pItems + newItemCount, memory::g_pInPlace ) Item{ item };
                ++newItemcount;
            }

            m_ItemCount = newItemCount;
        }

        template < typename... Arguments >
        void EmplaceBack( Arguments &&...arguments )
        {
            Reserve( m_ItemCount + 1 );
            ::new ( m_pItems + m_ItemCount, memory::g_pInPlace ) Item{ std::forward< Arguments >( arguments )... };
            ++m_ItemCount;
        }

        void PopBack()
        {
            SM_ASSERT( m_ItemCount );

            --m_ItemCount;

            m_pItems[m_ItemCount].~Item();
        }

        void Reserve( const Count itemCount, const bool shrinkAllocatedMemory = false )
        {
            Count reserved = GetReserved();

            if ( itemCount <= reserved && !shrinkAllocatedMemory )
                return;

            Item *pNewItems;

            if ( itemCount > reserved )
            {
                reserved = reserved + reserved / 2;

                if ( reserved < itemCount )
                {
                    reserved = itemCount;
                }
            }
            else
            {
                reserved = itemCount;
            }

            memory::AllocateUninitializedArray( pNewItems, reserved );

            if ( m_pItems )
            {
                memory::ConstructMoveArrayItems( pNewItems, m_ItemCount, m_pItems );
                memory::DestructArrayItems( m_pItems, m_ItemCount );
                memory::DeallocateByteArray( m_pItems );
            }

            m_pItems = pNewItems;
        }

      private:
        Item *m_pItems;
        Count m_ItemCount;
    };
}