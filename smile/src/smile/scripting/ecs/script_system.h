/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/ecs/timed_system.h"

namespace smile::world
{
    class World;
}

namespace smile::scripting::ecs
{
    class ScriptSystem final : public smile::ecs::TimedSystem< ScriptSystem >
    {
      public:
        ScriptSystem() = default;

        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnRemove( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate( primitive::Timestep deltaTime );
    };
}