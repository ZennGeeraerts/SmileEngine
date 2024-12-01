/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "group_base.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    GroupBase::GroupBase( ECSEngine &engine ) : m_Engine{ engine }
    {
    }

    void GroupBase::AddEntity( EntityHandleType entityHandle )
    {
        const IndexType entityIndex = entityHandle.GetIndex();

        if ( HasEntity( entityIndex ) )
        {
            for ( auto pComponent : m_pOwnedPools )
            {
                IndexType index = pComponent->m_Pool.GetIndex( entityIndex );
                pComponent->m_Pool.Swap( pComponent->m_Pool.GetElement( m_EndIndex ), entityIndex );
                pComponent->m_pComponentStorage->Swap( m_EndIndex, index );
            }

            ++m_EndIndex;

            for ( auto pOnEntityAddedListener : m_pOnEntityAddedListeners )
                ( *pOnEntityAddedListener )( entityHandle );
        }
    }

    void GroupBase::RemoveEntity( EntityHandleType entityHandle )
    {
        const IndexType entityIndex = entityHandle.GetIndex();

        if ( HasEntity( entityIndex ) )
        {
            for ( auto pComponent : m_pOwnedPools )
            {
                IndexType index = pComponent->m_Pool.GetIndex( entityIndex );
                pComponent->m_Pool.Swap( pComponent->m_Pool.GetElement( m_EndIndex - 1 ), entityIndex );
                pComponent->m_pComponentStorage->Swap( m_EndIndex - 1, index );
            }

            --m_EndIndex;

            for ( auto pOnEntityRemovedListener : m_pOnEntityRemovedListeners )
                ( *pOnEntityRemovedListener )( entityHandle );
        }
    }

    bool GroupBase::HasComponent( ComponentInterface *pComponent ) const
    {
        return ( std::find( m_pOwnedPools.begin(), m_pOwnedPools.end(), pComponent ) != m_pOwnedPools.end() ) ||
               ( std::find( m_pGetPools.begin(), m_pGetPools.end(), pComponent ) != m_pGetPools.end() );
    }

    GroupIterator GroupBase::begin() const
    {
        if ( !m_pOwnedPools.empty() )
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->m_Pool.begin() };
        else
            return GroupIterator{ m_Engine, SparseSetType::ConstIterator{} };
    }

    GroupIterator GroupBase::end() const
    {
        if ( !m_pOwnedPools.empty() )
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->m_Pool.begin() + m_EndIndex };
        else
            return GroupIterator{ m_Engine, SparseSetType::ConstIterator{} };
    }
}