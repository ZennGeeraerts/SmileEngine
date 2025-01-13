/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/core/ecs/system.h"
#include "engine/graphic/camera/camera.h"

namespace smile::ecs
{
    class ECSEngine;
}

namespace smile::graphic::ecs
{
    class CameraSystem final : public smile::ecs::System
    {
      public:
        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate() override;
    };
}