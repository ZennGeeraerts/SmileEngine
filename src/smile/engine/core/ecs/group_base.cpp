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
        if ( HasEntity( entityHandle ) )
        {
            for ( auto pComponent : m_pOwnedPools )
            {
                pComponent->Swap( pComponent->GetIndex( entityHandle ), m_EndIndex - 1 );
            }

            ++m_EndIndex;
        }
    }

    void GroupBase::RemoveEntity( EntityHandleType entityHandle )
    {
        if ( HasEntity( entityHandle ) )
        {
            for ( auto pComponent : m_pOwnedPools )
            {
                pComponent->Swap( pComponent->GetIndex( entityHandle ), m_EndIndex - 1 );
            }

            --m_EndIndex;
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
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->begin() };
        else
            return GroupIterator{ m_Engine, ComponentInterface::ConstIterator{} };
    }

    GroupIterator GroupBase::end() const
    {
        if ( !m_pOwnedPools.empty() )
            return GroupIterator{ m_Engine, ( *m_pOwnedPools.begin() )->begin() + m_EndIndex };
        else
            return GroupIterator{ m_Engine, ComponentInterface::ConstIterator{} };
    }
}