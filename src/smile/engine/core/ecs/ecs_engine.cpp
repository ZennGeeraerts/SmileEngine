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

        for ( auto pPool : m_pComponentPools )
            delete pPool;

        for ( auto pGroup : m_pGroups )
            delete pGroup;
    }

    void ECSEngine::DestroyEntity( EntityHandleType entityHandle )
    {
        for ( auto pPool : m_pComponentPools )
        {
            if ( HasComponent( pPool, entityHandle ) )
                RemoveComponent( pPool, entityHandle );
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

    void ECSEngine::RemoveComponent( ComponentPool *pPool, EntityHandleType entityHandle )
    {
        void *pComponentData = pPool->GetRaw( entityHandle );

        if ( pComponentData == nullptr )
            return;

        CallDestructors( pPool, pComponentData );

        for ( auto &pGroup : m_pGroups )
        {
            if ( pGroup->ContainsComponentPool( pPool ) )
                pGroup->RemoveEntity( entityHandle );
        }

        const IndexType deadEIndex = pPool->m_SparseSet.GetIndex( entityHandle.GetIndex() );
        pPool->m_pComponentStorage->RemoveSwap( deadEIndex );
        pPool->m_SparseSet.Erase( entityHandle.GetIndex() );
    }

    bool ECSEngine::HasComponent( const ComponentPool *pPool, EntityHandleType entityHandle ) const
    {
        return pPool ? pPool->Contains( entityHandle ) : false;
    }

    bool ECSEngine::IsComponentOwned( const ComponentPool *pPool ) const
    {
        return std::any_of( m_pGroups.cbegin(),
            m_pGroups.cend(),
            [pPool]( const GroupBase *pGroup )
            {
                const auto &pOwnedPools = pGroup->GetOwnedPools();
                return std::find( pOwnedPools.cbegin(), pOwnedPools.cend(), pPool ) != pOwnedPools.end();
            } );
    }

    void ECSEngine::CallDestructors( ComponentPool *pPool, void *pData )
    {
        for ( auto destructor : pPool->m_Destroy )
            destructor( pData );
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
        for ( auto pPool : m_pComponentPools )
        {
            for ( Uint32 i{}; i < pPool->m_pComponentStorage->GetSize(); ++i )
            {
                CallDestructors( pPool, pPool->m_pComponentStorage->GetRaw( i ) );
            }

            pPool->Clear();
        }

        while ( !m_pSystems.empty() )
            RemoveSystem( m_pSystems.back() );

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        m_pGroups.clear();
    }
}