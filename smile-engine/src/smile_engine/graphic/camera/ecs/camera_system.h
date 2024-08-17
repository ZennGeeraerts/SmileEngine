/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/core/ecs/system.h"
#include "smile_engine/graphic/camera/camera.h"

namespace smile::graphic::ecs
{
    class CameraSystem final : public smile::ecs::System
    {
      public:
        void OnUpdate( primitive::Timestep deltaTime ) override;
    };
}