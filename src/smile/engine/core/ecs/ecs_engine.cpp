/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "ecs_engine.h"

#include "system.h"

namespace smile::ecs
{
    ECSEngine::~ECSEngine()
    {
        while ( !m_pSystems.empty() )
            RemoveSystem( m_pSystems.back() );

        for ( auto pCPool : m_pComponentPools )
            delete pCPool;

        for ( auto pGroup : m_pGroups )
            delete pGroup;
    }

    void ECSEngine::DestroyEntity( EntityHandleType entityHandle )
    {
        for ( auto pCPool : m_pComponentPools )
        {
            if ( HasComponent( pCPool, entityHandle ) )
                RemoveComponent( pCPool, entityHandle );
        }

        m_HandleManager.DestroyEntity( entityHandle );
    }

    void ECSEngine::OnUpdate()
    {
        for ( auto pSystem : m_pSystems )
        {
            pSystem->OnUpdate();
        }

        for ( auto deadHandle : m_DeadHandles )
        {
            DestroyEntity( deadHandle );
        }

        m_DeadHandles.clear();
    }

    void ECSEngine::RemoveComponent( ComponentPool *pCPool, EntityHandleType entityHandle )
    {
        for ( auto &pGroup : m_pGroups )
        {
            if ( pGroup->ContainsComponentPool( pCPool ) )
                pGroup->RemoveEntity( entityHandle );
        }

        pCPool->Remove( entityHandle );
    }

    bool ECSEngine::HasComponent( const ComponentPool *pCPool, EntityHandleType entityHandle ) const
    {
        return pCPool ? pCPool->Contains( entityHandle ) : false;
    }

    bool ECSEngine::IsComponentOwned( const ComponentPool *pCPool ) const
    {
        return std::any_of( m_pGroups.cbegin(),
            m_pGroups.cend(),
            [pCPool]( const GroupBase *pGroup )
            {
                const auto &pOwnedPools = pGroup->GetOwnedPools();
                return std::find( pOwnedPools.cbegin(), pOwnedPools.cend(), pCPool ) != pOwnedPools.end();
            } );
    }

    void ECSEngine::AddSystem( Ref< BaseSystem > pSystem )
    {
        m_pSystems.emplace_back( pSystem );
        pSystem->OnAdd( *this );
    }

    void ECSEngine::RemoveSystem( Ref< BaseSystem > pSystem )
    {
        m_pSystems.erase( std::remove( m_pSystems.begin(), m_pSystems.end(), pSystem ) );
        pSystem->OnRemove( *this );
    }

    void ECSEngine::Clear()
    {
        for ( auto pCPool : m_pComponentPools )
        {
            pCPool->Clear();
        }

        while ( !m_pSystems.empty() )
            RemoveSystem( m_pSystems.back() );

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        m_pGroups.clear();
    }
}