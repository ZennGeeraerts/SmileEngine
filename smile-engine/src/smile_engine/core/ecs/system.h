/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "ecs_engine.h"

namespace smile::ecs
{
    class System
    {
      public:
        System() = default;
        System( ECSEngine *pECSEngine ) : m_pECSEngine{ pECSEngine }
        {
        }

        virtual ~System() = default;

        virtual void OnUpdate( primitive::Timestep deltaTime ) = 0;

      protected:
        ECSEngine *m_pECSEngine = nullptr;

        friend class ECSEngine;
    };
}