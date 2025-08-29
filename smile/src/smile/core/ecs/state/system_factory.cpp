/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "system_factory.h"

namespace smile::ecs::state
{
    primitive::HashMap< primitive::String, SystemProvider * > SystemFactory::s_SystemProviderMap{};

    memory::Ref< BaseSystem > SystemFactory::Create( const primitive::String &systemName )
    {
        return s_SystemProviderMap.GetItemAtKey( systemName )->Create();
    }

    void SystemFactory::RegisterSystem( const primitive::String &systemName, SystemProvider *pSystemProvider )
    {
        s_SystemProviderMap.TryInsert( systemName, pSystemProvider );
    }
}