/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "ecs_engine.h"

namespace smile::ecs
{
    ECSEngine::~ECSEngine()
    {
        for ( auto pComponentInterface : m_pComponents )
            delete pComponentInterface;

        for ( auto pGroup : m_pGroups )
            delete pGroup;
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

    void ECSEngine::CallDestructors( ComponentInterface *pComponentInterface, void *pData )
    {
        for ( auto destructor : pComponentInterface->m_Destroy )
            destructor( pData );
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