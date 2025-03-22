/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "state.h"
#include "smile/core/ecs/system.h"
#include "smile/common/memory/ref.h"

#include <unordered_map>

namespace smile::ecs
{
    class ECSEngine;

    namespace state
    {
        class StateManager final
        {
          public:
            StateManager() = default;
            ~StateManager() = default;

            void Initialize( ECSEngine *pECSEngine, const std::string &initialState );
            void ShutDown();

            void AddState( const std::string &name, memory::Ref< State > pState );
            bool HasState( const std::string &name ) const;
            void ChangeState( const std::string &name );

            std::vector< std::string > GetStates() const;
            State &GetState( const std::string &name ) const;
            memory::Ref< BaseSystem > GetSystem( const std::string &name ) const;

            static StateManager Copy( const StateManager &stateManager, ECSEngine *pECSEngine );

          private:
            memory::Ref< BaseSystem > GetOrCreateSystem( const std::string &systemName );
            void SyncState( const std::string &name,
                State::Iterator currentSystemsBegin,
                State::Iterator currentSystemsEnd,
                State::Iterator targetSystemsBegin,
                State::Iterator targetSystemsEnd );

          private:
            ECSEngine *m_pECSEngine = nullptr;
            std::unordered_map< std::string, memory::Ref< State > > m_StateMap;
            std::unordered_map< std::string, memory::Ref< BaseSystem > > m_SystemMap;

            memory::Ref< State > m_pCurrentState;
        };
    }
}