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
        for ( auto pComponentInterface : m_pComponents )
            delete pComponentInterface;

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        for ( auto pSystem : m_pSystems )
            delete pSystem;
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

    void ECSEngine::OnUpdate( Timestep deltaTime )
    {
        for ( auto pSystem : m_pSystems )
        {
            pSystem->OnUpdate( deltaTime );
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
                pGroup->RemoveEntity( entityHandle.GetIndex() );
        }

        const IndexType deadIndex = pComponentInterface->m_Pool.Erase( entityHandle.GetIndex() );
        pComponentInterface->m_pComponentStorage->RemoveSwap( deadIndex );

        // if ( pComponentInterface->m_IsRelational )
        // RelationalRebuild( pComponentInterface, deadIndex );
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
        pSystem->m_pECSEngine = this;
        m_pSystems.push_back( pSystem );
    }

    void ECSEngine::RemoveSystem( System *pSystem )
    {
        m_pSystems.erase( std::remove( m_pSystems.begin(), m_pSystems.end(), pSystem ) );
        delete pSystem;
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

        for ( auto pGroup : m_pGroups )
            delete pGroup;

        m_pGroups.clear();
    }
}