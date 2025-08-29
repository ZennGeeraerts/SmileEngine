/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "type_registry.h"

namespace smile::foundation
{
    TypeID TypeRegistry::RegisterType( const foundation::ConstantText typeName )
    {
        TypeID id{ typeName };
        m_IDToNameMap[id] = typeName;
        m_NameToIDMap[typeName] = id;

        return id;
    }

    TypeID TypeRegistry::RegisterTypeIfNeeded( const foundation::ConstantText typeName )
    {
        auto it = m_NameToIDMap.FindItemAtKey( typeName );
        if ( it != m_NameToIDMap.end() )
            return it.GetItem();

        return RegisterType( typeName );
    }
}