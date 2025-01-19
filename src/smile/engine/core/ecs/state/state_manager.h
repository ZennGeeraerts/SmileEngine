/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "state.h"
#include "ecs/system.h"

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

            void AddState( const std::string &name, Ref< State > pState );
            bool HasState( const std::string &name ) const;
            void ChangeState( const std::string &name );

            std::vector< std::string > GetStates() const;
            State &GetState( const std::string &name ) const;
            Ref< BaseSystem > GetSystem( const std::string &name ) const;

          private:
            Ref< BaseSystem > GetOrCreateSystem( const std::string &systemName );

          private:
            ECSEngine *m_pECSEngine;
            std::unordered_map< std::string, Ref< State > > m_StateMap;
            std::unordered_map< std::string, Ref< BaseSystem > > m_SystemMap;

            Ref< State > m_pCurrentState;
        };
    }
}