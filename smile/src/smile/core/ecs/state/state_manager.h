/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "state.h"
#include "system_registry.h"
#include "smile/core/ecs/system.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"

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

            void Initialize( ECSEngine *pECSEngine,
                SystemRegistry *pSystemRegistry,
                const primitive::StringView initialState );

            void ShutDown();

            void AddState( const primitive::String &name, memory::Ref< State > pState );
            bool HasState( const primitive::StringView name ) const;
            void ChangeState( const primitive::StringView name );

            primitive::Vector< primitive::String > GetStates() const;
            State &GetState( const primitive::StringView name ) const;
            memory::Ref< BaseSystem > GetSystem( const primitive::StringView name ) const;

            static StateManager
            Copy( const StateManager &stateManager, ECSEngine *pECSEngine, SystemRegistry *pSystemRegistry );

          private:
            memory::Ref< BaseSystem > GetOrCreateSystem( const primitive::String &systemName );

            primitive::Vector< primitive::String > TopologicalSort(
                const primitive::Vector< primitive::String > &systemNames );

          private:
            ECSEngine *m_pECSEngine = nullptr;
            SystemRegistry *m_pSystemRegistry = nullptr;
            primitive::HashMap< primitive::String, memory::Ref< State > > m_StateMap;
            primitive::HashMap< primitive::String, memory::Ref< BaseSystem > > m_SystemMap;

            memory::Ref< State > m_pCurrentState;
        };
    }
}