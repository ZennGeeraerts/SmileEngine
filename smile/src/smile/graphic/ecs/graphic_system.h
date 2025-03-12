/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/ecs/system.h"

namespace smile::graphic::ecs
{
    class GraphicSystem final : public smile::ecs::System< GraphicSystem >
    {
      public:
        GraphicSystem() = default;

        void OnUpdate() override;
    };
}