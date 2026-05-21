/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "vector.h"
#include "array_utils.h"

namespace smile::primitive
{
    template < typename KeyType, typename ItemType >
    class Dictionary final
    {
      public:
        struct IteratorValue final
        {
            const KeyType &Key;
            ItemType &Item;
        };

        class Iterator final
        {
          public:
            Iterator() = default;

            ItemType *operator->() const noexcept
            {
                return &m_pCollection->m_Items[m_Index];
            }

            ItemType *operator->() noexcept
            {
                return &m_pCollection->m_Items[m_Index];
            }

            IteratorValue operator*() const noexcept
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            IteratorValue operator*() noexcept
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            Iterator &operator++()
            {
                ++m_Index;

                SM_ASSERT( m_Index <= m_pCollection->m_Items.GetItemCount() );

                return *this;
            }

            bool operator==( const Iterator &other ) const noexcept
            {
                return m_pCollection == other.m_pCollection && m_Index == other.m_Index;
            }

            bool operator!=( const Iterator &other ) const noexcept
            {
                return m_pCollection != other.m_pCollection || m_Index != other.m_Index;
            }

            ItemType &GetItem() const noexcept
            {
                return m_pCollection->m_Items[m_Index];
            }

            const KeyType &GetKey() const noexcept
            {
                return m_pCollection->m_Keys[m_Index];
            }

            bool IsValid() const noexcept
            {
                return m_pCollection && m_pCollection->IsValidIndex( m_Index );
            }

          private:
            Iterator( Dictionary< KeyType, ItemType > *pCollection, const Index index )
                : m_Index{ index }, m_pCollection{ pCollection }
            {
            }

            Index m_Index{ s_InvalidIndex };
            Dictionary< KeyType, ItemType > *m_pCollection{ nullptr };

            friend class Dictionary;
        };

        struct ConstIteratorValue final
        {
            const KeyType &Key;
            const ItemType &Item;
        };

        class ConstIterator final
        {
          public:
            ConstIterator() noexcept = default;

            ConstIterator( const Iterator &other ) noexcept
                : m_Index{ other.m_Index }, m_pCollection{ other.m_pCollection }
            {
            }

            const ItemType *operator->() const noexcept
            {
                return &( m_pCollection->m_Items[m_Index] );
            }

            ConstIteratorValue operator*() const noexcept
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            ConstIterator &operator++()
            {
                ++m_Index;

                SM_ASSERT( m_Index <= m_pCollection->m_Items.GetItemCount() );

                return *this;
            }

            bool operator==( const ConstIterator &other ) const noexcept
            {
                return m_pCollection == other.m_pCollection && m_Index == other.m_Index;
            }

            bool operator!=( const ConstIterator &other ) const noexcept
            {
                return m_pCollection != other.m_pCollection || m_Index != other.m_Index;
            }

            const ItemType &GetItem() const noexcept
            {
                return m_pCollection->m_Items[m_Index];
            }

            const KeyType &GetKey() const noexcept
            {
                return m_pCollection->m_Keys[m_Index];
            }

            bool IsValid() const noexcept
            {
                return m_pCollection && m_pCollection->IsValidIndex( m_Index );
            }

          private:
            ConstIterator( const Dictionary< KeyType, ItemType > *pCollection, const Index index )
                : m_Index{ index }, m_pCollection{ pCollection }
            {
            }

            Index m_Index{ s_InvalidIndex };
            const Dictionary< KeyType, ItemType > *m_pCollection{ nullptr };

            friend class Dictionary;
        };

        Dictionary() noexcept = default;
        ~Dictionary() noexcept = default;

        ItemType &operator[]( const KeyType &key ) noexcept
        {
            return GetItemAtKey( key );
        }

        const ItemType &operator[]( const KeyType &key ) const noexcept
        {
            return GetItemAtKey( key );
        }

        const Vector< KeyType > &GetKeys() const noexcept
        {
            return m_Keys;
        }

        const Vector< ItemType > &GetItems() const noexcept
        {
            return m_Items;
        }

        ItemType &GetItemAtKey( const KeyType &key )
        {
            auto index = array::FindItemIndex( m_Keys, key );

            SM_ASSERT( index.has_value() );

            return m_Items[index.value()];
        }

        const ItemType &GetItemAtKey( const KeyType &key ) const
        {
            auto index = array::FindItemIndex( m_Keys, key );

            SM_ASSERT( index.has_value() );

            return m_Items[index.value()];
        }

        template < typename OtherKeyType >
        [[nodiscard]] bool HasItemAtKey( const OtherKeyType &key ) const noexcept
        {
            return array::HasItem( m_Keys, key );
        }

        Count GetItemCount() const noexcept
        {
            return m_Keys.GetItemCount();
        }

        Iterator Add( const KeyType &key, const ItemType &item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            m_Keys.PushBack( key );
            m_Items.PushBack( item );

            return { this, m_Items.GetLastIndex() };
        }

        Iterator Add( const KeyType &key, ItemType &&item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            m_Keys.PushBack( key );
            m_Items.PushBack( std::move( item ) );

            return { this, m_Items.GetLastIndex() };
        }

        void AddItems( const Dictionary &dict )
        {
#if SM_C_DEBUG
            for ( const auto &key : dict.m_Keys )
            {
                SM_ASSERT( !array::HasItem( m_Keys, key ) );
            }
#endif

            m_Keys.PushBack( dict.m_Keys );
            m_Items.PushBack( dict.m_Items );
        }

        template < typename OtherKeyType >
        Iterator FindItem( const OtherKeyType &key )
        {
            auto index = array::FindItemIndex( m_Keys, key );

            if ( index.has_value() )
            {
                return { this, index.value() };
            }
            else
            {
                return end();
            }
        }

        template < typename OtherKeyType >
        ConstIterator FindItem( const OtherKeyType &key ) const
        {
            auto index = array::FindItemIndex( m_Keys, key );

            if ( index.has_value() )
            {
                return { this, index.value() };
            }
            else
            {
                return end();
            }
        }

        void EraseAtKey( const KeyType &key )
        {
            auto keyIndex = array::FindItemIndex( m_Keys, key );

            SM_ASSERT_MSG( keyIndex.has_value(), "Item not found" );

            m_Keys.EraseAtIndex( keyIndex.value() );
            m_Items.EraseAtIndex( keyIndex.value() );
        }

        void Erase( Iterator &iterator )
        {
            SM_ASSERT( iterator.IsValid() );

            m_Keys.EraseAtIndex( iterator.m_Index );
            m_Items.EraseAtIndex( iterator.m_Index );

            iterator = {};
        }

        void Clear() noexcept
        {
            m_Keys.Clear();
            m_Items.Clear();
        }

        bool IsEmpty() const noexcept
        {
            return m_Keys.IsEmpty();
        }

        void Reserve( const Count itemCount ) noexcept
        {
            m_Keys.Reserve( itemCount );
            m_Items.Reserve( itemCount );
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
            return { this, GetItemCount() };
        }

        ConstIterator end() const noexcept
        {
            return { this, GetItemCount() };
        }

      private:
        bool IsValidIndex( const Index index ) const noexcept
        {
            return m_Keys.IsValidIndex( index );
        }

      private:
        Vector< KeyType > m_Keys;
        Vector< ItemType > m_Items;
    };
}