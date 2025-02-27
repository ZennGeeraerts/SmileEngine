/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "state_manager.h"

#include "ecs/ecs_engine.h"
#include "system_factory.h"

namespace smile::ecs::state
{
    void StateManager::Initialize( ECSEngine *pECSEngine, const std::string &initialState )
    {
        m_pECSEngine = pECSEngine;

        SM_ASSERT( m_StateMap.find( initialState ) != m_StateMap.end(),
            "StateManager::Initialize > Initial state not found in state map" );

        m_pCurrentState = m_StateMap.at( initialState );
        const std::vector< std::string > &systemNames = m_pCurrentState->GetSystemNames();

        for ( const auto &systemName : systemNames )
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

    void StateManager::ChangeState( const std::string &name, const std::vector< std::string > &systemsAtBack )
    {
        SM_ASSERT( HasState( name ), "StateManager::ChangeState > State manager does not have state" );

        const auto &pTargetState = m_StateMap.at( name );

        std::vector< std::string > currentSystems = m_pCurrentState->GetSystemNames();
        std::vector< std::string > targetSystems = pTargetState->GetSystemNames();
        std::vector< std::string > toBeRemovedSystems{ systemsAtBack };
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

        toBeAddedSystems.insert( toBeAddedSystems.end(), systemsAtBack.begin(), systemsAtBack.end() );

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

        for ( const auto &toBeAdded : toBeAddedSystems )
        {
            m_pECSEngine->AddSystem( GetOrCreateSystem( toBeAdded ) );
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
}