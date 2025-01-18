/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace smile::ecs::state
{
    class State final
    {
      public:
        State() = default;
        ~State() = default;

        void AddSystem( const std::string &systemName );
        bool HasSystem( std::string_view systemName );

        const std::vector< std::string > &GetSystemNames() const
        {
            return m_SystemNames;
        }

      private:
        std::vector< std::string > m_SystemNames;
    };
}