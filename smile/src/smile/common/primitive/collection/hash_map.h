/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "hash_table.h"

namespace smile::primitive
{
    template < typename TKey, typename TValue >
    class HashMap final
    {
      public:
        struct Node final
        {
            using KeyType = TKey;
            using ItemType = TValue;

            Node() = delete;

            template < typename OtherKeyType >
            Node( OtherKeyType &&key ) : Key{ std::forward< OtherKeyType >( key ) }, Value{}
            {
            }

            template < typename OtherKeyType, typename OtherItemType >
            Node( OtherKeyType &&key, OtherItemType &&item )
                : Key{ std::forward< OtherKeyType >( key ) }, Value{ std::forward< OtherItemType >( item ) }
            {
            }

            const KeyType &GetKey() const noexcept
            {
                return Key;
            }

            const ItemType &GetItem() const noexcept
            {
                return Value;
            }

            ItemType &GetItem() noexcept
            {
                return Value;
            }

            Node *GetNextNode() const noexcept
            {
                return pNextNode;
            }

            void SetNextNode( Node *pNode ) noexcept
            {
                pNextNode = pNode;
            }

            ItemType Value;
            KeyType Key;
            Node *pNextNode;
            foundation::HashCode Hash{ 0 };
        };

        using Iterator = typename HashTable< Node >::Iterator;
        using ConstIterator = typename HashTable< Node >::ConstIterator;

        HashMap() = default;

        HashMap( const HashMap &other )
        {
            for ( const auto &item : other )
            {
                Insert( item.Key, item.Value );
            }
        }

        HashMap( HashMap &&other ) noexcept : m_Table{ std::move( other.m_Table ) }
        {
        }

        HashMap( std::initializer_list< std::pair< TKey, TValue > > values )
        {
            for ( const auto &item : values )
            {
                Insert( item.first, item.second );
            }
        }

        ~HashMap()
        {
            Clear();
        }

        HashMap &operator=( const HashMap &other )
        {
            Clear();

            for ( const auto &item : other )
            {
                Insert( item.Key, item.Value );
            }

            return *this;
        }

        HashMap &operator=( HashMap &&other ) noexcept
        {
            Clear();
            m_Table.Swap( other.m_Table );

            return *this;
        }

        TValue &operator[]( const TKey &key )
        {
            auto [iterator, _] = TryCreateItemAtKey( key );
            return iterator.GetItem();
        }

        const TValue &operator[]( const TKey &key ) const
        {
            return m_Table.GetItemAtKey( key );
        }

        Count GetItemCount() const noexcept
        {
            return m_Table.GetItemCount();
        }

        bool IsEmpty() const noexcept
        {
            return m_Table.IsEmpty();
        }

        ConstIterator FindItemAtKey( const TKey &key ) const
        {
            return m_Table.FindItemAtKey( key );
        }

        template < typename OtherKeyType >
        ConstIterator FindItemAtKey( const OtherKeyType &key ) const
        {
            return m_Table.FindItemAtKey( key );
        }

        Iterator FindItemAtKey( const TKey &key )
        {
            return m_Table.FindItemAtKey( key );
        }

        template < typename OtherKeyType >
        Iterator FindItemAtKey( const OtherKeyType &key )
        {
            return m_Table.FindItemAtKey( key );
        }

        [[nodiscard]] bool HasItemAtKey( const TKey &key ) const
        {
            return m_Table.HasItemAtKey( key );
        }

        const TValue &GetItemAtKey( const TKey &key ) const
        {
            return m_Table.GetItemAtKey( key );
        }

        TValue &GetItemAtKey( const TKey &key )
        {
            return m_Table.GetItemAtKey( key );
        }

        Iterator CreateItemAtKey( const TKey &key )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            auto pNode = new Node{ key };
            m_Table.Insert( pNode );

            return Iterator{ pNode, &m_Table };
        }

        std::pair< Iterator, bool > TryCreateItemAtKey( const TKey &key )
        {
            auto it = FindItemAtKey( key );

            if ( it == end() )
            {
                auto pNode = new Node{ key };

                m_Table.Insert( pNode );

                return std::make_pair( Iterator{ pNode, &m_Table }, true );
            }

            return std::make_pair( it, false );
        }

        Iterator Insert( const TKey &key, const TValue &item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            auto pNewNode = new Node{ key, item };

            m_Table.Insert( pNewNode );

            return { pNewNode, &m_Table };
        }

        std::pair< Iterator, bool > TryInsert( const TKey &key, const TValue &item )
        {
            auto it = FindItemAtKey( key );

            if ( it == end() )
            {
                auto pNewNode = new Node{ key, item };

                m_Table.Insert( pNewNode );

                return std::make_pair( Iterator{ pNewNode, &m_Table }, true );
            }

            return std::make_pair( it, false );
        }

        Iterator Insert( const TKey &key, TValue &&item )
        {
            SM_ASSERT( !HasItemAtKey( key ) );

            auto pNewNode = new Node{ key, std::move( item ) };

            m_Table.Insert( pNewNode );

            return { pNewNode, &m_Table };
        }

        std::pair< Iterator, bool > TryInsert( const TKey &key, TValue &&item )
        {
            auto it = FindItemAtKey( key );

            if ( it == end() )
            {
                auto pNewNode = new Node{ key, std::move( item ) };

                m_Table.Insert( pNewNode );

                return std::make_pair( Iterator{ pNewNode, &m_Table }, true );
            }

            return std::make_pair( it, false );
        }

        void InsertItems( const HashMap &other )
        {
            for ( const auto &item : other )
            {
                Insert( item.Key, item.Value );
            }
        }

        void EraseItemAtKey( const TKey &key )
        {
            delete m_Table.EraseItemAtKey( key );
        }

        void Erase( Iterator &where )
        {
            delete m_Table.Erase( where );
        }

        void EraseAndSetNext( Iterator &where )
        {
            delete m_Table.EraseAndSetNext( where );
        }

        void Clear() noexcept
        {
            auto iterator = m_Table.begin();

            while ( iterator != m_Table.end() )
            {
                delete m_Table.EraseAndSetNext( iterator );
            }
        }

        Iterator begin()
        {
            return m_Table.begin();
        }

        Iterator end() noexcept
        {
            return m_Table.end();
        }

        ConstIterator begin() const
        {
            return m_Table.begin();
        }

        ConstIterator end() const noexcept
        {
            return m_Table.end();
        }

      private:
        HashTable< Node > m_Table;
    };
}