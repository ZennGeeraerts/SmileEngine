/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "type_registry.h"

namespace smile::foundation
{
    TypeID TypeRegistry::RegisterType( const std::string &typeName )
    {
        TypeID id{ typeName };
        m_IDToNameMap[id] = typeName;
        m_NameToIDMap[typeName] = id;

        return id;
    }

    TypeID TypeRegistry::RegisterTypeIfNeeded( const std::string &typeName )
    {
        auto it = m_NameToIDMap.find( typeName );
        if ( it != m_NameToIDMap.end() )
            return it->second;

        return RegisterType( typeName );
    }
}