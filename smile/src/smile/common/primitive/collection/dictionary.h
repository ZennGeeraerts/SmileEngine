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

            inline ItemType *operator->() const
            {
                return &m_pCollection->m_Items[m_Index];
            }

            inline ItemType *operator->()
            {
                return &m_pCollection->m_Items[m_Index];
            }

            inline IteratorValue operator*() const
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            inline IteratorValue operator*()
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            inline Iterator &operator++()
            {
                ++m_Index;

                SM_ASSERT( m_Index <= m_pCollection->m_Items.GetItemCount() );

                return *this;
            }

            inline bool operator==( const Iterator &other ) const
            {
                return m_pCollection == other.m_pCollection && m_Index == other.m_Index;
            }

            inline bool operator!=( const Iterator &other ) const
            {
                return m_pCollection != other.m_pCollection || m_Index != other.m_Index;
            }

            inline ItemType &GetItem() const
            {
                return m_pCollection->m_Items[m_Index];
            }

            inline const KeyType &GetKey() const
            {
                return m_pCollection->m_Keys[m_Index];
            }

            inline bool IsValid() const
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
            inline ConstIterator() = default;

            inline ConstIterator( const Iterator &other )
                : m_Index{ other.m_Index }, m_pCollection{ other.m_pCollection }
            {
            }

            inline const ItemType *operator->() const
            {
                return &( m_pCollection->m_Items[m_Index] );
            }

            inline ConstIteratorValue operator*() const
            {
                return { m_pCollection->m_Keys[m_Index], m_pCollection->m_Items[m_Index] };
            }

            inline ConstIterator &operator++()
            {
                ++m_Index;

                SM_ASSERT( m_Index <= m_pCollection->m_Items.GetItemCount() );

                return *this;
            }

            inline bool operator==( const ConstIterator &other ) const
            {
                return m_pCollection == other.m_pCollection && m_Index == other.m_Index;
            }

            inline bool operator!=( const ConstIterator &other ) const
            {
                return m_pCollection != other.m_pCollection || m_Index != other.m_Index;
            }

            inline const ItemType &GetItem() const
            {
                return m_pCollection->m_Items[m_Index];
            }

            inline const KeyType &GetKey() const
            {
                return m_pCollection->m_Keys[m_Index];
            }

            inline bool IsValid() const
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

        inline Dictionary() noexcept = default;
        inline ~Dictionary() noexcept = default;

        ItemType &operator[]( const KeyType &key )
        {
            return GetItemAtKey( key );
        }

        const ItemType &operator[]( const KeyType &key ) const
        {
            return GetItemAtKey( key );
        }

        const Vector< KeyType > &GetKeys() const
        {
            return m_Keys;
        }

        const Vector< ItemType > &GetItems() const
        {
            return m_Items;
        }

        inline ItemType &GetItemAtKey( const KeyType &key )
        {
            auto index = array::FindItemIndex( m_Keys, key );

            SM_ASSERT( index.has_value() );

            return m_Items[index.value()];
        }

        inline const ItemType &GetItemAtKey( const KeyType &key ) const
        {
            auto index = array::FindItemIndex( m_Keys, key );

            SM_ASSERT( index.has_value() );

            return m_Items[index.value()];
        }

        template < typename OtherKeyType >
        inline bool HasItemAtKey( const OtherKeyType &key ) const
        {
            return array::HasItem( m_Keys, key );
        }

        inline Count GetItemCount() const
        {
            return m_Keys.GetItemCount();
        }

        inline Iterator Add( const KeyType &key, const ItemType &item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            m_Keys.PushBack( key );
            m_Items.PushBack( item );

            return { this, m_Items.GetLastIndex() };
        }

        inline Iterator Add( const KeyType &key, ItemType &&item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            m_Keys.PushBack( key );
            m_Items.PushBack( std::move( item ) );

            return { this, m_Items.GetLastIndex() };
        }

        inline void AddItems( const Dictionary &dict )
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

        inline void Clear()
        {
            m_Keys.Clear();
            m_Items.Clear();
        }

        inline bool IsEmpty() const
        {
            return m_Keys.IsEmpty();
        }

        inline void Reserve( const Count itemCount )
        {
            m_Keys.Reserve( itemCount );
            m_Items.Reserve( itemCount );
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
            return { this, GetItemCount() };
        }

        inline ConstIterator end() const
        {
            return { this, GetItemCount() };
        }

      private:
        bool IsValidIndex( const Index index ) const
        {
            return m_Keys.IsValidIndex( index );
        }

      private:
        Vector< KeyType > m_Keys;
        Vector< ItemType > m_Items;
    };
}