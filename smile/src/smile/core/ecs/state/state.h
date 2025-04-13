/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"

#include <string>
#include <string_view>
#include <vector>

namespace smile::ecs::state
{
    class State final : public memory::Counted
    {
      public:
        using Iterator = std::vector< std::string >::iterator;

      public:
        State() = default;
        ~State() = default;

        void AddSystem( std::string systemName );
        bool HasSystem( std::string_view systemName );

        const std::vector< std::string > &GetSystemNames() const
        {
            return m_SystemNames;
        }

      private:
        std::vector< std::string > m_SystemNames;
    };
}