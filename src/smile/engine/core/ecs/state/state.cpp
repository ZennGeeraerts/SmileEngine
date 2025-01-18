/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "state.h"

namespace smile::ecs::state
{
    void State::AddSystem( const std::string &systemName )
    {
        m_SystemNames.emplace_back( systemName );
    }

    bool State::HasSystem( std::string_view systemName )
    {
        return std::find( m_SystemNames.begin(), m_SystemNames.end(), systemName ) != m_SystemNames.end();
    }
}