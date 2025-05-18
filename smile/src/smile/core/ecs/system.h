/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "base_system.h"
#include "smile/common/foundation/type_name.h"

namespace smile::ecs
{
    template< typename DerivedType >
    class System : public BaseSystem
    {
      public:
        System() = default;
        virtual ~System() = default;

        virtual void OnAdd( ECSEngine &ecsEngine )
        {
            m_pECSEngine = &ecsEngine;
        };

        virtual void OnRemove( ECSEngine &ecsEngine )
        {
            SM_ASSERT_MSG( m_pECSEngine == &ecsEngine, "System does not belong to this ecs engine" );
            m_pECSEngine = nullptr;
        };

        std::string_view GetName() const override
        {
            return GetStaticName();
        }

        static std::string_view GetStaticName()
        {
            return foundation::TypeNameOf< DerivedType >();
        }

      protected:
        ECSEngine *m_pECSEngine = nullptr;
    };
}