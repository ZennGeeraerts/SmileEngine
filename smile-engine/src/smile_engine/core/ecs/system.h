/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "base_system.h"

namespace smile::ecs
{
    class ECSEngine;

	class System : public BaseSystem
	{
      public:
        System() = default;
        virtual ~System() = default;

        virtual void OnAdd( ECSEngine &ecsEngine ) override
        {
            m_pECSEngine = &ecsEngine;
        };

        virtual void OnRemove( ECSEngine &ecsEngine ) override
        {
            SM_ASSERT( m_pECSEngine == &ecsEngine, "System does not belong to this ecs engine" );
            m_pECSEngine = nullptr;
        };

        virtual void OnUpdate() = 0;

      protected:
        ECSEngine *m_pECSEngine = nullptr;
	};
}