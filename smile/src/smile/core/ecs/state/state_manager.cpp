/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "state_manager.h"

#include "smile/core/ecs/ecs_engine.h"
#include "system_factory.h"

#include <queue>

namespace smile::ecs::state
{
    void
    StateManager::Initialize( ECSEngine *pECSEngine, SystemRegistry *pSystemRegistry, const std::string &initialState )
    {
        m_pECSEngine = pECSEngine;
        m_pSystemRegistry = pSystemRegistry;

        SM_ASSERT_MSG( m_StateMap.find( initialState ) != m_StateMap.end(),
            "StateManager::Initialize > Initial state not found in state map" );

        m_pCurrentState = m_StateMap.at( initialState );
        const std::vector< std::string > &systemNames = m_pCurrentState->GetSystemNames();

        std::vector< std::string > sorted = TopologicalSort( systemNames );

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

    void StateManager::AddState( const std::string &name, memory::Ref< State > pState )
    {
        m_StateMap.insert( std::make_pair( name, pState ) );
    }

    bool StateManager::HasState( const std::string &name ) const
    {
        return m_StateMap.find( name ) != m_StateMap.end();
    }

    void StateManager::ChangeState( const std::string &name )
    {
        SM_ASSERT_MSG( HasState( name ), "StateManager::ChangeState > State manager does not have state" );

        const auto &pTargetState = m_StateMap.at( name );

        std::vector< std::string > currentSystems = m_pCurrentState->GetSystemNames();
        std::vector< std::string > targetSystems = pTargetState->GetSystemNames();
        std::vector< std::string > toBeRemovedSystems{};
        std::vector< std::string > toBeAddedSystems{};

        std::sort( currentSystems.begin(), currentSystems.end() );
        std::sort( targetSystems.begin(), targetSystems.end() );

        std::set_difference( currentSystems.begin(),
            currentSystems.end(),
            targetSystems.begin(),
            targetSystems.end(),
            std::inserter( toBeRemovedSystems, toBeRemovedSystems.end() ) );

        std::set_difference( targetSystems.begin(),
            targetSystems.end(),
            currentSystems.begin(),
            currentSystems.end(),
            std::inserter( toBeAddedSystems, toBeAddedSystems.end() ) );

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

        std::vector< std::string > sorted = TopologicalSort( targetSystems );

        for ( const std::string &systemName : sorted )
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

    std::vector< std::string > StateManager::GetStates() const
    {
        std::vector< std::string > states;

        for ( const auto &pair : m_StateMap )
        {
            states.emplace_back( pair.first );
        }

        return states;
    }

    State &StateManager::GetState( const std::string &name ) const
    {
        return *m_StateMap.at( name );
    }

    memory::Ref< BaseSystem > StateManager::GetSystem( const std::string &name ) const
    {
        return m_SystemMap.at( name );
    }

    memory::Ref< BaseSystem > StateManager::GetOrCreateSystem( const std::string &systemName )
    {
        auto systemIt = m_SystemMap.find( systemName );

        if ( systemIt == m_SystemMap.end() )
        {
            auto pNewSystem = SystemFactory::Create( systemName );
            m_SystemMap.insert( std::make_pair( systemName, pNewSystem ) );
            return pNewSystem;
        }
        else
        {
            return systemIt->second;
        }
    }

    std::vector< std::string > StateManager::TopologicalSort( const std::vector< std::string > &systemNames )
    {
        std::unordered_map< std::string, int > inDegree{};
        std::unordered_map< std::string, std::vector< std::string > > adjacencyList{};

        for ( const std::string &name : systemNames )
        {
            inDegree[name] = 0;
        }

        for ( const std::string &name : systemNames )
        {
            const auto &systemInfo = m_pSystemRegistry->GetSystemInfo( name );

            for ( const std::string &dependency : systemInfo.GetDependencies() )
            {
                if ( std::find( systemNames.begin(), systemNames.end(), dependency ) == systemNames.end() )
                    continue;

                adjacencyList[dependency].push_back( name );
                ++inDegree[name];
            }
        }

        std::queue< std::string > resolveDependencyQueue{};

        for ( const auto &[name, degree] : inDegree )
        {
            if ( degree == 0 )
                resolveDependencyQueue.push( name );
        }

        std::vector< std::string > sorted{};

        while ( !resolveDependencyQueue.empty() )
        {
            auto current = resolveDependencyQueue.front();
            resolveDependencyQueue.pop();
            sorted.push_back( current );

            for ( const std::string &neighbor : adjacencyList[current] )
            {
                if ( --inDegree[neighbor] == 0 )
                {
                    resolveDependencyQueue.push( neighbor );
                }
            }
        }

        SM_ASSERT_MSG( sorted.size() == systemNames.size(),
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

        const std::vector< std::string > &systemNames = result.m_pCurrentState->GetSystemNames();
        std::vector< std::string > sorted = result.TopologicalSort( systemNames );

        for ( const auto &systemName : sorted )
        {
            auto pSystem = result.GetOrCreateSystem( systemName );
            result.m_pECSEngine->AddSystem( pSystem );
        }

        return result;
    }
}