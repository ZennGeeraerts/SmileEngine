/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/range_iterator.h"
#include "smile/common/foundation/hash_code.h"
#include "smile/common/memory/memory.h"

#include <type_traits>

namespace smile::primitive
{
    template < typename Type >
    concept NodeTypeConcept = requires( Type node, Type *pNode ) {
        typename Type::KeyType;
        typename Type::ItemType;

        {
            node.GetKey()
        } -> std::convertible_to< const typename Type::KeyType & >;
        {
            node.GetItem()
        };
        {
            node.GetNextNode()
        } -> std::convertible_to< Type * >;
        {
            node.SetNextNode( pNode )
        } -> std::same_as< void >;

        requires std::is_same_v< decltype( node.Hash ), foundation::HashCode >;
    };

    template < NodeTypeConcept NodeType >
    class HashTable final
    {
      public:
        using KeyType = NodeType::KeyType;
        using ItemType = NodeType::ItemType;

        template < bool IsConst >
        class BaseIterator final
        {
          public:
            using IteratorItem = typename std::conditional_t< IsConst, const ItemType, ItemType >;
            using IteratorCollection =
                typename std::conditional_t< IsConst, const HashTable< NodeType >, HashTable< NodeType > >;
            using IteratorNode = typename std::conditional_t< IsConst, const NodeType, NodeType >;
            using IteratorItemInPair =
                std::conditional_t< std::is_reference_v< decltype( std::declval< NodeType >().GetItem() ) >,
                    IteratorItem &,
                    IteratorItem >;

            struct IteratorValue final
            {
                const KeyType &Key;
                IteratorItemInPair Value;
            };

            BaseIterator() noexcept = default;

            BaseIterator( IteratorNode *pNode, IteratorCollection *pCollection ) noexcept
                : m_pCollection{ pCollection }, m_pNode{ pNode }
            {
            }

            IteratorValue operator*() const
            {
                return { m_pNode->GetKey(), m_pNode->GetItem() };
            }

            BaseIterator &operator++()
            {
                m_pCollection->GetNext( *this );

                return *this;
            }

            bool operator==( const BaseIterator &other ) const noexcept
            {
                return m_pNode == other.m_pNode;
            }

            bool operator!=( const BaseIterator &other ) const noexcept
            {
                return m_pNode != other.m_pNode;
            }

            void SetNode( IteratorNode *pNode ) noexcept
            {
                m_pNode = pNode;
            }

            IteratorItem &GetItem()
            {
                return m_pNode->GetItem();
            }

            const KeyType &GetKey() const
            {
                return m_pNode->GetKey();
            }

          private:
            IteratorCollection *m_pCollection{ nullptr };
            IteratorNode *m_pNode{ nullptr };

            friend class HashTable< NodeType >;
        };

        using Iterator = BaseIterator< false >;
        using ConstIterator = BaseIterator< true >;

        HashTable() noexcept : m_ppBuckets{ nullptr }, m_ItemCount{ 0 }
        {
            m_ppBuckets = new NodeType *[static_cast< std::size_t >( s_BucketSize )];
            memory::FillByteArray( m_ppBuckets,
                0,
                static_cast< Count >( sizeof( NodeType * ) * static_cast< std::size_t >( s_BucketSize ) ) );
        }

        HashTable( const HashTable & ) = delete;

        HashTable( HashTable &&other ) noexcept
        {
            Swap( other );
        }

        ~HashTable() noexcept
        {
            Clear();
            delete[] m_ppBuckets;
        }

        HashTable &operator=( const HashTable & ) = delete;

        HashTable &operator=( HashTable &&other ) noexcept
        {
            Swap( other );

            return *this;
        }

        Count GetItemCount() const noexcept
        {
            return m_ItemCount;
        }

        bool IsEmpty() const noexcept
        {
            return m_ItemCount == 0;
        }

        void GetNext( Iterator &iterator )
        {
            auto pNode = GetNextNode( iterator.m_pNode );

            iterator.SetNode( pNode );
        }

        void GetNext( ConstIterator &iterator ) const
        {
            auto pNode = GetNextNode( iterator.m_pNode );

            iterator.SetNode( pNode );
        }

        Iterator FindItemAtKey( const KeyType &key )
        {
            auto pNode = FindNodeAtKey( key );

            return { pNode, this };
        }

        template < typename OtherKeyType >
        Iterator FindItemAtKey( const OtherKeyType &key )
        {
            return FindItemAtKey( KeyType{ key } );
        }

        ConstIterator FindItemAtKey( const KeyType &key ) const
        {
            auto pNode = FindNodeAtKey( key );

            return { pNode, this };
        }

        template < typename OtherKeyType >
        ConstIterator FindItemAtKey( const OtherKeyType &key ) const
        {
            return FindItemAtKey( KeyType{ key } );
        }

        [[nodiscard]] bool HasItemAtKey( const KeyType &key ) const
        {
            return FindNodeAtKey( key ) != nullptr;
        }

        const ItemType &GetItemAtKey( const KeyType &key ) const
        {
            auto pNode = FindNodeAtKey( key );

            SM_ASSERT( pNode );

            return pNode->GetItem();
        }

        ItemType &GetItemAtKey( const KeyType &key )
        {
            auto pNode = FindNodeAtKey( key );

            SM_ASSERT( pNode );

            return pNode->GetItem();
        }

        [[nodiscard]] bool ContainsHashCode( const foundation::HashCode hashCode ) const
        {
            auto bucketIndex = hashCode & s_BucketMask;

            const NodeType *pCurrentNode = m_ppBuckets[bucketIndex];

            while ( pCurrentNode )
            {
                if ( pCurrentNode->HashCode == hashCode )
                {
                    return true;
                }

                pCurrentNode = pCurrentNode->GetNextNode();
            }

            return false;
        }

        void Insert( NodeType *pNode )
        {
            SM_ASSERT( !HasItemAtKey( pNode->GetKey() ) );

            pNode->Hash = std::hash< KeyType >{}( pNode->GetKey() );

            auto bucketIndex = pNode->Hash & s_BucketMask;
            pNode->SetNextNode( m_ppBuckets[bucketIndex] );
            m_ppBuckets[bucketIndex] = pNode;

            ++m_ItemCount;
        }

        NodeType *EraseItemAtKey( const KeyType &key )
        {
            auto pNodeToErase = FindNodeAtKey( key );

            SM_ASSERT( pNodeToErase );

            EraseNode( pNodeToErase );

            return pNodeToErase;
        }

        NodeType *Erase( Iterator &where )
        {
            SM_ASSERT( where.m_pNode );

            EraseNode( where.m_pNode );
            where.SetNode( nullptr );

            return where.m_pNode;
        }

        void EraseNode( NodeType *pNodeToErase )
        {
            SM_ASSERT( pNodeToErase );
            SM_ASSERT( pNodeToErase->Hash == std::hash< KeyType >{}( pNodeToErase->GetKey() ) );

            auto bucketIndex = pNodeToErase->Hash & s_BucketMask;

            if ( m_ppBuckets[bucketIndex] == pNodeToErase )
            {
                m_ppBuckets[bucketIndex] = m_ppBuckets[bucketIndex]->GetNextNode();
            }
            else
            {
                NodeType *pCurrentNode = m_ppBuckets[bucketIndex];

                while ( pCurrentNode->GetNextNode() != pNodeToErase )
                {
                    pCurrentNode = pCurrentNode->GetNextNode();
                }

                pCurrentNode->SetNextNode( pNodeToErase->GetNextNode() );
            }

            pNodeToErase->SetNextNode( nullptr );

            --m_ItemCount;
        }

        NodeType *EraseAndSetNext( Iterator &where )
        {
            SM_ASSERT( where != end() );

            auto pNodeToErase = where.m_pNode;

            where.m_pNode = GetNextNode( pNodeToErase );

            EraseNode( pNodeToErase );

            return pNodeToErase;
        }

        void Clear() noexcept
        {
            NodeType *pCurrentNode{ nullptr };

            for ( auto bucketIndex : foundation::GetCountIterator( s_BucketSize ) )
            {
                pCurrentNode = m_ppBuckets[bucketIndex];

                while ( pCurrentNode )
                {
                    auto pNextNode = pCurrentNode->GetNextNode();
                    pCurrentNode->SetNextNode( nullptr );
                    pCurrentNode = pNextNode;
                }
            }

            m_ItemCount = 0;

            memory::FillByteArray( m_ppBuckets,
                0,
                static_cast< Count >( sizeof( NodeType * ) * static_cast< std::size_t >( s_BucketSize ) ) );
        }

        void Swap( HashTable &other ) noexcept
        {
            std::swap( m_ppBuckets, other.m_ppBuckets );
            std::swap( m_ItemCount, other.m_ItemCount );
        }

        Iterator begin()
        {
            if ( !IsEmpty() )
            {
                for ( auto bucketIndex : foundation::GetCountIterator( s_BucketSize ) )
                {
                    if ( m_ppBuckets[bucketIndex] )
                    {
                        return { m_ppBuckets[bucketIndex], this };
                    }
                }
            }

            return { nullptr, this };
        }

        ConstIterator begin() const
        {
            if ( !IsEmpty() )
            {
                for ( auto bucketIndex : foundation::GetCountIterator( s_BucketSize ) )
                {
                    if ( m_ppBuckets[bucketIndex] )
                    {
                        return { m_ppBuckets[bucketIndex], this };
                    }
                }
            }

            return { nullptr, this };
        }

        Iterator end() noexcept
        {
            return { nullptr, this };
        }

        ConstIterator end() const noexcept
        {
            return { nullptr, this };
        }

      private:
        NodeType *GetNextNode( NodeType *pNode )
        {
            if ( !pNode->GetNextNode() )
            {
                auto bucketIndex = ( pNode->Hash & s_BucketMask ) + 1;

                while ( bucketIndex < s_BucketSize && !m_ppBuckets[bucketIndex] )
                {
                    ++bucketIndex;
                }

                if ( bucketIndex < s_BucketSize )
                {
                    return m_ppBuckets[bucketIndex];
                }
                else
                {
                    return nullptr;
                }
            }

            return pNode->GetNextNode();
        }

        const NodeType *GetNextNode( const NodeType *pNode ) const
        {
            if ( !pNode->GetNextNode() )
            {
                auto bucketIndex = ( pNode->Hash & s_BucketMask ) + 1;

                while ( bucketIndex < s_BucketSize && !m_ppBuckets[bucketIndex] )
                {
                    ++bucketIndex;
                }

                if ( bucketIndex < s_BucketSize )
                {
                    return m_ppBuckets[bucketIndex];
                }
                else
                {
                    return nullptr;
                }
            }

            return pNode->GetNextNode();
        }

        NodeType *FindNodeAtKey( const KeyType &key )
        {
            auto hashCode = std::hash< KeyType >{}( key );
            auto bucketIndex = hashCode & s_BucketMask;

            auto pCurrentNode = m_ppBuckets[bucketIndex];

            while ( pCurrentNode )
            {
                if ( pCurrentNode->Hash == hashCode && pCurrentNode->GetKey() == key )
                {
                    return pCurrentNode;
                }

                pCurrentNode = pCurrentNode->GetNextNode();
            }

            return nullptr;
        }

        const NodeType *FindNodeAtKey( const KeyType &key ) const
        {
            auto hashCode = std::hash< KeyType >{}( key );
            auto bucketIndex = hashCode & s_BucketMask;

            auto pCurrentNode = m_ppBuckets[bucketIndex];

            while ( pCurrentNode )
            {
                if ( pCurrentNode->Hash == hashCode && pCurrentNode->GetKey() == key )
                {
                    return pCurrentNode;
                }

                pCurrentNode = pCurrentNode->GetNextNode();
            }

            return nullptr;
        }

      private:
        NodeType **m_ppBuckets;
        Count m_ItemCount;

        static constexpr Uint32 s_DefaultBucketExponent = 4;
        static constexpr Count s_BucketSize = 1 << s_DefaultBucketExponent;
        static constexpr Uint32 s_BucketMask = ( 1 << s_DefaultBucketExponent ) - 1;
    };
}