/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "default_system_provider.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/hash_map.h"

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

            static memory::Ref< BaseSystem > Create( const primitive::String &systemName );
            static void RegisterSystem( const primitive::String &systemName, SystemProvider *pSystemProvider );

            template < typename SystemType, typename... Args >
            static void RegisterSystem( Args... args )
            {
                SystemFactory::RegisterSystem( primitive::String{ SystemType::GetStaticName() },
                    new DefaultSystemProvider< SystemType, Args... >{ args... } );
            }

          private:
            static primitive::HashMap< primitive::String, SystemProvider * > s_SystemProviderMap;
        };
    }
}