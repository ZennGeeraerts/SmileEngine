/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "group_base.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    GroupBase::GroupBase( ECSEngine &engine,
        const std::vector< ComponentPool * > &pOwned,
        const std::vector< ComponentPool * > &pGet )
        : m_Engine{ engine }, m_pOwnedPools{ pOwned }, m_pGetPools{ pGet }
    {
    }

    void GroupBase::AddEntity( EntityHandle entityHandle )
    {
        if ( ContainsEntity( entityHandle ) && m_EndIndex != 0 )
        {
            for ( auto pCPool : m_pOwnedPools )
            {
                pCPool->Swap( pCPool->GetIndex( entityHandle ), m_EndIndex - 1 );
            }

            ++m_EndIndex;
        }
    }

    void GroupBase::RemoveEntity( EntityHandle entityHandle )
    {
        if ( ContainsEntity( entityHandle ) )
        {
            for ( auto pCPool : m_pOwnedPools )
            {
                pCPool->Swap( pCPool->GetIndex( entityHandle ), m_EndIndex - 1 );
            }

            --m_EndIndex;
        }
    }

    bool GroupBase::ContainsComponentPool( ComponentPool *pPool ) const
    {
        return ( std::find( m_pOwnedPools.begin(), m_pOwnedPools.end(), pPool ) != m_pOwnedPools.end() ) ||
               ( std::find( m_pGetPools.begin(), m_pGetPools.end(), pPool ) != m_pGetPools.end() );
    }

    GroupIterator GroupBase::begin() const
    {
        if ( !m_pOwnedPools.empty() )
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->begin() };
        else
            return GroupIterator{ m_Engine, ComponentPool::ConstIterator{} };
    }

    GroupIterator GroupBase::end() const
    {
        if ( !m_pOwnedPools.empty() )
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->begin() + m_EndIndex };
        else
            return GroupIterator{ m_Engine, ComponentPool::ConstIterator{} };
    }
}