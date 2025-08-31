/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "state.h"

namespace smile::ecs::state
{
    void State::AddSystem( primitive::String systemName )
    {
        m_SystemNames.EmplaceBack( std::move( systemName ) );
    }

    bool State::HasSystem( const primitive::StringView systemName )
    {
        return std::find( m_SystemNames.begin(), m_SystemNames.end(), systemName ) != m_SystemNames.end();
    }
}