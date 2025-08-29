/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "system_factory.h"
#include "smile/common/memory/counted.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"

namespace smile::ecs::state
{
    class SystemInfo final
    {
      public:
        SystemInfo() = default;

        SystemInfo &After( const primitive::String &systemName )
        {
            m_Dependencies.PushBack( systemName );
            return *this;
        }

        const primitive::Vector< primitive::String > &GetDependencies() const
        {
            return m_Dependencies;
        }

      private:
        primitive::Vector< primitive::String > m_Dependencies{};
    };

    class SystemRegistry final : public memory::Counted
    {
      public:
        template < typename SystemType, typename... Args >
        SystemInfo &RegisterSystem( Args &&...args )
        {
            SystemFactory::RegisterSystem< SystemType >( std::forward< Args >( args )... );

            const primitive::String systemName{ SystemType::GetStaticName() };
            auto [iterator, _] = m_SystemInfoMap.TryCreateItemAtKey( systemName );
            return iterator.GetItem();
        }

        const SystemInfo &GetSystemInfo( const primitive::StringView systemName ) const
        {
            SM_ASSERT_MSG( m_SystemInfoMap.HasItemAtKey( systemName ),
                "SystemRegistry::GetSystemInfo > Failed to find system: {}",
                systemName );

            return m_SystemInfoMap[systemName];
        }

      private:
        primitive::HashMap< primitive::String, SystemInfo > m_SystemInfoMap;
    };
}