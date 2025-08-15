/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"
#include "smile/common/foundation/type_id.h"
#include "smile/common/memory/scope.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "component_serializer.h"

namespace smile::world
{
    class ComponentSerializerRegistry final : public foundation::MeyersSingleton< ComponentSerializerRegistry >
    {
      public:
        using ComponentSerializerMap =
            primitive::HashMap< foundation::TypeID, memory::Scope< ComponentSerializerBase > >;

        ComponentSerializerRegistry() = default;

        template < typename ComponentType >
        void RegisterComponentSerializerIfNeeded()
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            auto it = m_ComponentSerializers.FindItemAtKey( typeID );

            if ( it == m_ComponentSerializers.end() )
            {
                return;
            }

            auto pComponentSerializer = memory::CreateScope< ComponentSerializer< ComponentType > >();
            m_ComponentSerializers.Insert( typeID, std::move( pComponentSerializer ) );
        }

        const ComponentSerializerMap &GetComponentSerializers() const
        {
            return m_ComponentSerializers;
        }

      private:
        ComponentSerializerMap m_ComponentSerializers;
    };
}