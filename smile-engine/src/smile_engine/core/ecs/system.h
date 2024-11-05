/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::ecs
{
    class ECSEngine;

    class System
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
            SM_ASSERT( m_pECSEngine == &ecsEngine, "System does not belong to this ecs engine" );
            m_pECSEngine = nullptr;
        };

        virtual void OnUpdate() = 0;

      protected:
        ECSEngine *m_pECSEngine = nullptr;
    };
}