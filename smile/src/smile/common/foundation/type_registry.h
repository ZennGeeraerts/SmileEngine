/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "meyers_singleton.h"
#include "type_id.h"

namespace smile::foundation
{
    class TypeRegistry final : public MeyersSingleton< TypeRegistry >
    {
      public:
        template < typename Type >
        constexpr TypeID RegisterType()
        {
            TypeID id = TypeIDOf< Type >();
            std::string typeName = TypeNameOf< Type >();

            m_IDToNameMap[id] = typeName;
            m_NameToIDMap[typeName] = id;

            return id;
        }

        template < typename Type >
        TypeID RegisterTypeIfNeeded()
        {
            auto it = m_NameToIDMap.find( typeName );
            if ( it != m_NameToIDMap.end() )
                return it->second;

            return RegisterType< Type >();
        }

        TypeID RegisterType( const std::string &typeName );
        TypeID RegisterTypeIfNeeded( const std::string &typeName );

        bool Contains( TypeID typeID ) const
        {
            return m_IDToNameMap.find( typeID ) != m_IDToNameMap.end();
        }

        std::string_view GetName( TypeID typeID ) const
        {
            SM_ASSERT_MSG( Contains( typeID ), "TypeRegistry::GetName > ID not found" );
            return m_IDToNameMap[typeID];
        }

      private:
        mutable std::unordered_map< std::string, TypeID > m_NameToIDMap;
        mutable std::unordered_map< TypeID, std::string > m_IDToNameMap;
    };
}