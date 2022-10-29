#pragma once

#include <algorithm>

namespace Smile::ECS
{
    class View final
    {
      public:
        struct Iterator final
        {
            Iterator( EntityHandleManager &handleManager,
                SparseSetType::ConstIterator it,
                const std::vector< SparseSetType * > &pPools )
                : HandleManager{ handleManager }, It{ it }, pPools{ pPools }
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

                SparseSetType *pMinPools = *std::min_element( std::begin( pPools ),
                    std::end( pPools ),
                    []( SparseSetType *pLhs, SparseSetType *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                if ( It == pMinPools->end() )
                    return *this;

                SparseSetType::ConstIterator oldIt;
                do
                {
                    oldIt = It;

                    for ( auto pPool : pPools )
                    {
                        if ( pPool == pMinPools )
                            continue;

                        if ( !pPool->Contains( *It ) )
                        {
                            ++It;
                            break;
                        }
                    }
                } while ( ( It != oldIt ) && ( HandleManager.GetEntityHandle( *It ).IsValid() ) );

                return *this;
            }

            EntityHandleManager &HandleManager;
            SparseSetType::ConstIterator It;
            std::vector< SparseSetType * > pPools;
        };

      public:
        View( EntityHandleManager &handleManager, const std::vector< ComponentInterface * > &pComponents )
            : m_HandleManager{ handleManager }
        {
            for ( auto pComponent : pComponents )
            {
                if ( pComponent )
                    m_pPools.push_back( &pComponent->m_Pool );
            }
        }

        const Iterator begin() const
        {
            if ( !m_pPools.empty() )
            {
                SparseSetType *pMinPool = *std::min_element( std::begin( m_pPools ),
                    std::end( m_pPools ),
                    []( SparseSetType *pLhs, SparseSetType *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                return Iterator{ m_HandleManager, pMinPool->begin(), m_pPools };
            }
            else
            {
                return Iterator{ m_HandleManager, SparseSetType::ConstIterator{}, m_pPools };
            }
        }

        const Iterator end() const
        {
            if ( !m_pPools.empty() )
            {
                SparseSetType *pMinPool = *std::min_element( std::begin( m_pPools ),
                    std::end( m_pPools ),
                    []( SparseSetType *pLhs, SparseSetType *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                return Iterator{ m_HandleManager, pMinPool->end(), m_pPools };
            }
            else
            {
                return Iterator{ m_HandleManager, SparseSetType::ConstIterator{}, m_pPools };
            }
        }

      private:
        EntityHandleManager &m_HandleManager;
        std::vector< SparseSetType * > m_pPools{};
    };
}