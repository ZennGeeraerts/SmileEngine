/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/core/ecs/system.h"

namespace smile::world
{
    class World;
}

namespace smile::ecs
{
    class ECSEngine;
}

namespace smile::world::ecs
{
    class TransformSystem final : public smile::ecs::System< TransformSystem >
    {
      public:
        TransformSystem() = default;

        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate() override;
    };
}