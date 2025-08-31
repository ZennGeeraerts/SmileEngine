/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "state_manager.h"

#include "smile/core/ecs/ecs_engine.h"
#include "system_factory.h"
#include "smile/common/primitive/collection/iterator/back_inserter.h"

#include <queue>

namespace smile::ecs::state
{
    void StateManager::Initialize( ECSEngine *pECSEngine,
        SystemRegistry *pSystemRegistry,
        const primitive::StringView initialState )
    {
        m_pECSEngine = pECSEngine;
        m_pSystemRegistry = pSystemRegistry;

        SM_ASSERT_MSG( m_StateMap.FindItemAtKey( initialState ) != m_StateMap.end(),
            "StateManager::Initialize > Initial state not found in state map" );

        m_pCurrentState = m_StateMap.GetItemAtKey( initialState );
        const primitive::Vector< primitive::String > &systemNames = m_pCurrentState->GetSystemNames();

        primitive::Vector< primitive::String > sorted = TopologicalSort( systemNames );

        for ( const auto &systemName : sorted )
        {
            auto pSystem = GetOrCreateSystem( systemName );
            m_pECSEngine->AddSystem( pSystem );
        }
    }

    void StateManager::ShutDown()
    {
        m_pECSEngine = nullptr;
    }

    void StateManager::AddState( const primitive::String &name, memory::Ref< State > pState )
    {
        m_StateMap.Insert( name, std::move( pState ) );
    }

    bool StateManager::HasState( const primitive::StringView name ) const
    {
        return m_StateMap.HasItemAtKey( name );
    }

    void StateManager::ChangeState( const primitive::StringView name )
    {
        SM_ASSERT_MSG( HasState( name ), "StateManager::ChangeState > State manager does not have state" );

        const auto &pTargetState = m_StateMap.GetItemAtKey( name );

        primitive::Vector< primitive::String > currentSystems = m_pCurrentState->GetSystemNames();
        primitive::Vector< primitive::String > targetSystems = pTargetState->GetSystemNames();
        primitive::Vector< primitive::String > toBeRemovedSystems{};
        primitive::Vector< primitive::String > toBeAddedSystems{};

        std::sort( currentSystems.begin(), currentSystems.end() );
        std::sort( targetSystems.begin(), targetSystems.end() );

        std::set_difference( currentSystems.begin(),
            currentSystems.end(),
            targetSystems.begin(),
            targetSystems.end(),
            primitive::BackInserter{ toBeRemovedSystems } );

        std::set_difference( targetSystems.begin(),
            targetSystems.end(),
            currentSystems.begin(),
            currentSystems.end(),
            primitive::BackInserter{ toBeAddedSystems } );

        const auto &pSystems = m_pECSEngine->GetSystems();

        for ( const auto &toBeRemoved : toBeRemovedSystems )
        {
            auto systemIt = std::find_if( pSystems.begin(),
                pSystems.end(),
                [&toBeRemoved]( const auto &pSystem ) { return pSystem->GetName() == toBeRemoved; } );

            if ( systemIt != pSystems.end() )
            {
                m_pECSEngine->RemoveSystem( *systemIt );
            }
            else
            {
                SM_LOG_WARNING( "StateManager::ChangeState > While transitionning to state {0}, System {1} was not "
                                "found, while the states difference reports as to be removed",
                    name,
                    toBeRemoved );
            }
        }

        primitive::Vector< primitive::String > sorted = TopologicalSort( targetSystems );

        for ( const primitive::String &systemName : sorted )
        {
            memory::Ref< BaseSystem > pSystem = GetOrCreateSystem( systemName );

            if ( std::find( toBeAddedSystems.begin(), toBeAddedSystems.end(), systemName ) != toBeAddedSystems.end() )
            {
                m_pECSEngine->AddSystem( pSystem );
            }
            else
            {
                m_pECSEngine->RemoveSystem( pSystem );
                m_pECSEngine->AddSystem( pSystem );
            }
        }

        m_pCurrentState = pTargetState;
    }

    primitive::Vector< primitive::String > StateManager::GetStates() const
    {
        primitive::Vector< primitive::String > states;

        for ( const auto &pair : m_StateMap )
        {
            states.PushBack( pair.Key );
        }

        return states;
    }

    State &StateManager::GetState( const primitive::StringView name ) const
    {
        return *m_StateMap.GetItemAtKey( name );
    }

    memory::Ref< BaseSystem > StateManager::GetSystem( const primitive::StringView name ) const
    {
        return m_SystemMap.GetItemAtKey( name );
    }

    memory::Ref< BaseSystem > StateManager::GetOrCreateSystem( const primitive::String &systemName )
    {
        auto systemIt = m_SystemMap.FindItemAtKey( systemName );

        if ( systemIt == m_SystemMap.end() )
        {
            auto pNewSystem = SystemFactory::Create( systemName );
            auto insertIt = m_SystemMap.Insert( systemName, std::move( pNewSystem ) );
            return insertIt.GetItem();
        }
        else
        {
            return systemIt.GetItem();
        }
    }

    primitive::Vector< primitive::String > StateManager::TopologicalSort(
        const primitive::Vector< primitive::String > &systemNames )
    {
        primitive::HashMap< primitive::String, int > inDegree{};
        primitive::HashMap< primitive::String, primitive::Vector< primitive::String > > adjacencyList{};

        for ( const primitive::String &name : systemNames )
        {
            inDegree[name] = 0;
        }

        for ( const primitive::String &name : systemNames )
        {
            const auto &systemInfo = m_pSystemRegistry->GetSystemInfo( name );

            for ( const primitive::String &dependency : systemInfo.GetDependencies() )
            {
                if ( std::find( systemNames.begin(), systemNames.end(), dependency ) == systemNames.end() )
                    continue;

                adjacencyList[dependency].PushBack( name );
                ++inDegree[name];
            }
        }

        std::queue< primitive::String > resolveDependencyQueue{};

        for ( const auto &[name, degree] : inDegree )
        {
            if ( degree == 0 )
                resolveDependencyQueue.push( name );
        }

        primitive::Vector< primitive::String > sorted{};

        while ( !resolveDependencyQueue.empty() )
        {
            auto current = resolveDependencyQueue.front();
            resolveDependencyQueue.pop();
            sorted.PushBack( current );

            for ( const primitive::String &neighbor : adjacencyList[current] )
            {
                if ( --inDegree[neighbor] == 0 )
                {
                    resolveDependencyQueue.push( neighbor );
                }
            }
        }

        SM_ASSERT_MSG( sorted.GetItemCount() == systemNames.GetItemCount(),
            "StateManager::TopologicalSort > Cycle detected or missing dependency" );

        return sorted;
    }

    StateManager
    StateManager::Copy( const StateManager &stateManager, ECSEngine *pECSEngine, SystemRegistry *pSystemRegistry )
    {
        StateManager result{};
        result.m_StateMap = stateManager.m_StateMap; // TODO: Make a deep copy instead
        result.m_pCurrentState = stateManager.m_pCurrentState;
        result.m_pECSEngine = pECSEngine;
        result.m_pSystemRegistry = pSystemRegistry;

        const primitive::Vector< primitive::String > &systemNames = result.m_pCurrentState->GetSystemNames();
        primitive::Vector< primitive::String > sorted = result.TopologicalSort( systemNames );

        for ( const auto &systemName : sorted )
        {
            auto pSystem = result.GetOrCreateSystem( systemName );
            result.m_pECSEngine->AddSystem( pSystem );
        }

        return result;
    }
}