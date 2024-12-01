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

        for ( auto pComponentInterface : m_pComponents )
            delete pComponentInterface;

        for ( auto pGroup : m_pGroups )
            delete pGroup;
    }

    void ECSEngine::DestroyEntity( EntityHandleType entityHandle )
    {
        for ( auto pComponentInterface : m_pComponents )
        {
            if ( HasComponent( pComponentInterface, entityHandle ) )
                RemoveComponent( pComponentInterface, entityHandle );
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

    void ECSEngine::RemoveComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle )
    {
        void *pComponentData = pComponentInterface->GetRaw( entityHandle );

        if ( pComponentData == nullptr )
            return;

        CallDestructors( pComponentInterface, pComponentData );

        for ( auto &pGroup : m_pGroups )
        {
            if ( pGroup->HasComponent( pComponentInterface ) )
                pGroup->RemoveEntity( entityHandle );
        }

        const IndexType deadEIndex = pComponentInterface->m_Pool.GetIndex( entityHandle.GetIndex() );
        pComponentInterface->m_pComponentStorage->RemoveSwap( deadEIndex );
        pComponentInterface->m_Pool.Erase( entityHandle.GetIndex() );
    }

    bool ECSEngine::HasComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle ) const
    {
        return pComponentInterface ? pComponentInterface->m_Pool.Contains( entityHandle.GetIndex() ) : false;
    }

    bool ECSEngine::IsComponentOwned( const ComponentInterface *pComponentInterface ) const
    {
        return std::any_of( m_pGroups.cbegin(),
            m_pGroups.cend(),
            [pComponentInterface]( GroupBase *pGroup )
            {
                const auto &pOwnedComponents = pGroup->GetOwnedComponents();
                return std::find( pOwnedComponents.cbegin(), pOwnedComponents.cend(), pComponentInterface ) !=
                       pOwnedComponents.end();
            } );
    }

    void ECSEngine::CallDestructors( ComponentInterface *pComponentInterface, void *pData )
    {
        for ( auto destructor : pComponentInterface->m_Destroy )
            destructor( pData );
    }

    void ECSEngine::AddSystem( System *pSystem )
    {
        m_pSystems.push_back( pSystem );
        pSystem->OnAdd( *this );
    }

    void ECSEngine::RemoveSystem( System *pSystem )
    {
        m_pSystems.erase( std::remove( m_pSystems.begin(), m_pSystems.end(), pSystem ) );
        pSystem->OnRemove( *this );
    }

    void ECSEngine::Clear()
    {
        for ( auto pComponentInterface : m_pComponents )
        {
            for ( Uint32 i{}; i < pComponentInterface->m_pComponentStorage->GetSize(); ++i )
            {
                CallDestructors( pComponentInterface, pComponentInterface->m_pComponentStorage->GetRaw( i ) );
            }

            pComponentInterface->Clear();
        }

        while ( !m_pSystems.empty() )
            RemoveSystem( m_pSystems.back() );

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        m_pGroups.clear();
    }
}