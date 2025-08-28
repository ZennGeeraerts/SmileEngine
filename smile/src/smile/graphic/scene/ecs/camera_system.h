/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/core/ecs/system.h"

namespace smile::ecs
{
    class ECSEngine;
}

namespace smile::graphic::ecs
{
    class CameraSystem final : public smile::ecs::System< CameraSystem >
    {
      public:
        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnRemove( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate() override;
    };
}