/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "system_factory.h"
#include "smile/common/memory/counted.h"

namespace smile::ecs::state
{
    class SystemInfo final
    {
      public:
        SystemInfo() = default;

        SystemInfo &After( const std::string &systemName )
        {
            m_Dependencies.push_back( systemName );
            return *this;
        }

        const std::vector< std::string > &GetDependencies() const
        {
            return m_Dependencies;
        }

      private:
        std::vector< std::string > m_Dependencies{};
    };

    class SystemRegistry final : public memory::Counted
    {
      public:
        template < typename SystemType, typename... Args >
        SystemInfo &RegisterSystem( Args &&...args )
        {
            SystemFactory::RegisterSystem< SystemType >( std::forward< Args >( args )... );

            std::string systemName{ SystemType::GetStaticName() };
            const auto [pair, _] = m_SystemInfoMap.emplace( systemName, SystemInfo{} );
            return pair->second;
        }

        const SystemInfo &GetSystemInfo( const std::string &systemName ) const
        {
            SM_ASSERT( m_SystemInfoMap.find( systemName ) != m_SystemInfoMap.end(),
                "SystemRegistry::GetSystemInfo > Failed to find system: {}",
                systemName );

            return m_SystemInfoMap[systemName];
        }

      private:
        mutable std::unordered_map< std::string, SystemInfo > m_SystemInfoMap;
    };
}