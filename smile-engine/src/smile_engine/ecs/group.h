#pragma once

#include <algorithm>

namespace Smile::ECS
{
    class Group final
    {
      public:
        struct Iterator final
        {
            Iterator( EntityHandleManager &handleManager, SparseSetType::ConstIterator it )
                : HandleManager{ handleManager }, It{ it }
            {
            }

            EntityHandleType operator*() const
            {
                return HandleManager.GetEntityHandle( *It );
            }
            bool operator==( const Iterator &other ) const
            {
                return It == other.It || ( *It ) == HandleManager.GetEntityCount();
            }
            bool operator!=( const Iterator &other ) const
            {
                return It != other.It && ( *It ) != HandleManager.GetEntityCount();
            }
            Iterator &operator++()
            {
                ++It;
                return *this;
            }

            EntityHandleManager &HandleManager;
            SparseSetType::ConstIterator It
                ;
        };

      public:
        Group( EntityHandleManager &handleManager,
            const std::vector< ComponentInterface * > &pOwned,
            const std::vector< ComponentInterface * > &pGet )
            : m_HandleManager{ handleManager }
        {
            std::vector< SparseSetType * > pPools{};

            for ( auto pComponent : pOwned )
            {
                if ( pComponent )
                {
                    pPools.push_back( &pComponent->m_Pool );
                    m_pOwnedPools.push_back( pComponent );
                }
            }

            for ( auto pComponent : pGet )
            {
                if ( pComponent )
                {
                    pPools.push_back( &pComponent->m_Pool );
                    m_pGetPools.push_back( pComponent );
                }
            }

            if ( pPools.empty() )
                return;

            SparseSetType *pMinPool = *std::min_element( std::begin( pPools ),
                std::end( pPools ),
                []( SparseSetType *pLhs, SparseSetType *pRhs )
                { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

            for ( Uint32 i{}; i < pMinPool->GetItemCount(); ++i )
            {
                auto entity = pMinPool->GetElement( i );
                bool isMatch = true;

                for ( auto pPool : pPools )
                {
                    if ( pPool == pMinPool )
                        continue;

                    if ( !pPool->Contains( entity ) )
                    {
                        isMatch = false;
                        break;
                    }
                }

                if ( isMatch )
                {
                    for ( auto pPool : m_pOwnedPools )
                    {
                        IndexType index = pPool->m_Pool.GetIndex( entity );
                        pPool->m_Pool.Swap( pPool->m_Pool.GetElement( i ), entity );
                        pPool->m_pComponentStorage->Swap( i, index );
                    }

                    ++m_EndIndex;
                }
            }
        }

        void AddEntity( IndexType entityIndex )
        {
            if ( HasEntity( entityIndex ) )
            {
                for ( auto component : m_pOwnedPools )
                {
                    IndexType index = component->m_Pool.GetIndex( entityIndex );
                    component->m_Pool.Swap( component->m_Pool.GetElement( m_EndIndex ), entityIndex );
                    component->m_pComponentStorage->Swap( m_EndIndex, index );
                }

                ++m_EndIndex;
            }
        }

        void RemoveEntity( IndexType entityIndex )
        {
            if ( HasEntity( entityIndex ) )
            {
                for ( auto pComponent : m_pOwnedPools )
                {
                    IndexType index = pComponent->m_Pool.GetIndex( entityIndex );
                    pComponent->m_Pool.Swap( pComponent->m_Pool.GetElement( m_EndIndex - 1 ), entityIndex );
                    pComponent->m_pComponentStorage->Swap( m_EndIndex - 1, index );
                }

                --m_EndIndex;
            }
        }

        bool HasComponent( ComponentInterface *pComponent ) const
        {
            return ( std::find( m_pOwnedPools.begin(), m_pOwnedPools.end(), pComponent ) != m_pOwnedPools.end() ) ||
                   ( std::find( m_pGetPools.begin(), m_pGetPools.end(), pComponent ) != m_pGetPools.end() );
        }

        bool HasEntity( IndexType entityIndex ) const
        {
            std::vector< SparseSetType * > pPools{};

            for ( auto pComponent : m_pOwnedPools )
                pPools.push_back( &pComponent->m_Pool );
            for ( auto pComponent : m_pGetPools )
                pPools.push_back( &pComponent->m_Pool );

            for ( auto pPool : pPools )
            {
                if ( !pPool->Contains( entityIndex ) )
                    return false;
            }

            return true;
        }

        Iterator begin() const
        {
            if ( !m_pOwnedPools.empty() )
                return Iterator{ m_HandleManager, ( *m_pOwnedPools.begin() )->m_Pool.begin() };
            else
                return Iterator{ m_HandleManager, SparseSetType::ConstIterator{} };
        }

        Iterator end() const
        {
            if ( !m_pOwnedPools.empty() )
                return Iterator{ m_HandleManager, ( *m_pOwnedPools.begin() )->m_Pool.begin() + m_EndIndex };
            else
                return Iterator{ m_HandleManager, SparseSetType::ConstIterator{} };
        }

        const std::vector< ComponentInterface * > &GetOwnedComponents() const
        {
            return m_pOwnedPools;
        }
        const std::vector< ComponentInterface * > &GetGetComponents() const
        {
            return m_pGetPools;
        }

      private:
        EntityHandleManager &m_HandleManager;

        std::vector< ComponentInterface * > m_pOwnedPools{};
        std::vector< ComponentInterface * > m_pGetPools{};

        IndexType m_EndIndex{ 0 };
    };
}