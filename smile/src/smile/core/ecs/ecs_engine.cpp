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
        while ( !m_pSystems.IsEmpty() )
            RemoveSystem( m_pSystems.GetLastItem() );

        for ( auto pCPool : m_pComponentPools )
            delete pCPool;

        for ( auto pGroup : m_pGroups )
            delete pGroup;
    }

    void ECSEngine::DestroyEntity( EntityHandle entityHandle )
    {
        for ( auto pCPool : m_pComponentPools )
        {
            if ( HasComponent( pCPool, entityHandle ) )
                RemoveComponent( pCPool, entityHandle );
        }

        m_HandleManager.DestroyHandle( entityHandle );
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

        m_DeadHandles.Clear();
    }

    void ECSEngine::RemoveComponent( ComponentPool *pCPool, EntityHandle entityHandle )
    {
        for ( auto &pGroup : m_pGroups )
        {
            if ( pGroup->ContainsComponentPool( pCPool ) )
                pGroup->RemoveEntity( entityHandle );
        }

        pCPool->Remove( entityHandle );
    }

    bool ECSEngine::HasComponent( const ComponentPool *pCPool, EntityHandle entityHandle ) const
    {
        return pCPool ? pCPool->Contains( entityHandle ) : false;
    }

    bool ECSEngine::IsComponentOwned( const ComponentPool *pCPool ) const
    {
        return std::any_of( m_pGroups.begin(),
            m_pGroups.end(),
            [pCPool]( const GroupBase *pGroup )
            {
                const auto &pOwnedPools = pGroup->GetOwnedPools();
                return std::find( pOwnedPools.cbegin(), pOwnedPools.cend(), pCPool ) != pOwnedPools.end();
            } );
    }

    void ECSEngine::AddSystem( memory::Ref< BaseSystem > pSystem )
    {
        pSystem->OnAdd( *this );
        m_pSystems.PushBack( std::move( pSystem ) );
    }

    void ECSEngine::RemoveSystem( memory::Ref< BaseSystem > pSystem )
    {
        m_pSystems.Erase( std::remove( m_pSystems.begin(), m_pSystems.end(), pSystem ) );
        pSystem->OnRemove( *this );
    }

    void ECSEngine::Clear()
    {
        for ( auto pCPool : m_pComponentPools )
        {
            pCPool->Clear();
        }

        while ( !m_pSystems.IsEmpty() )
            RemoveSystem( m_pSystems.GetLastItem() );

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        m_pGroups.Clear();
    }
}