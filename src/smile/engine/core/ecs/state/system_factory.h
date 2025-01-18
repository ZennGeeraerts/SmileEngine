/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "ecs/base_system.h"
#include "default_system_provider.h"

namespace smile::ecs
{
    class BaseSystem;

    namespace state
    {
        class SystemFactory final
        {
          public:
            SystemFactory() = default;
            ~SystemFactory() = default;

            static Ref< BaseSystem > Create( const std::string &systemName );
            static void RegisterSystem( const std::string &systemName, SystemProvider *pSystemProvider );

            template < typename SystemType, typename... Args >
            static void RegisterSystem( Args... args )
            {
                SystemFactory::RegisterSystem( std::string{ SystemType::GetStaticName() },
                    new DefaultSystemProvider< SystemType, Args... >{ args... } );
            }

          private:
            static std::unordered_map< std::string, SystemProvider * > s_SystemProviderMap;
        };
    }
}