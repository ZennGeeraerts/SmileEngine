/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/core/ecs/system.h"

namespace smile::scene
{
    class Scene;
}

namespace smile::ecs
{
    class ECSEngine;
}

namespace smile::scene::ecs
{
    class TransformSystem final : public smile::ecs::System
    {
      public:
        TransformSystem( Scene *pScene );

        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate() override;

      private:
        Scene *m_pScene;
    };
}