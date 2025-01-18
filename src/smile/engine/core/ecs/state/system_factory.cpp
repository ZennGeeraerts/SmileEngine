/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "system_factory.h"

namespace smile::ecs::state
{
    std::unordered_map< std::string, SystemProvider * > SystemFactory::s_SystemProviderMap{};

    Ref< BaseSystem > SystemFactory::Create( const std::string &systemName )
    {
        return s_SystemProviderMap.at( systemName )->Create();
    }

    void SystemFactory::RegisterSystem( const std::string &systemName, SystemProvider *pSystemProvider )
    {
        s_SystemProviderMap.insert( std::make_pair( systemName, pSystemProvider ) );
    }
}