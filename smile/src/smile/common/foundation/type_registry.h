/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "meyers_singleton.h"
#include "type_id.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/text/string.h"

namespace smile::foundation
{
    class TypeRegistry final : public MeyersSingleton< TypeRegistry >
    {
      public:
        template < typename Type >
        constexpr TypeID RegisterType()
        {
            TypeID id = TypeIDOf< Type >();
            primitive::String typeName = TypeNameOf< Type >();

            m_IDToNameMap[id] = std::move( typeName );
            m_NameToIDMap[typeName] = id;

            return id;
        }

        template < typename Type >
        TypeID RegisterTypeIfNeeded()
        {
            auto typeName = foundation::TypeNameOf< Type >();

            auto it = m_NameToIDMap.FindItemAtKey( typeName );
            if ( it != m_NameToIDMap.end() )
                return it.GetValue();

            return RegisterType< Type >();
        }

        TypeID RegisterType( const foundation::ConstantText typeName );
        TypeID RegisterTypeIfNeeded( const foundation::ConstantText typeName );

        bool Contains( TypeID typeID ) const
        {
            return m_IDToNameMap.FindItemAtKey( typeID ) != m_IDToNameMap.end();
        }

        primitive::StringView GetName( TypeID typeID ) const
        {
            SM_ASSERT_MSG( Contains( typeID ), "TypeRegistry::GetName > ID not found" );
            return m_IDToNameMap[typeID];
        }

      private:
        primitive::HashMap< primitive::String, TypeID > m_NameToIDMap;
        primitive::HashMap< TypeID, primitive::String > m_IDToNameMap;
    };
}