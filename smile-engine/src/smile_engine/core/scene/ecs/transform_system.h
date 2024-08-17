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

namespace smile::scene::ecs
{
    class TransformSystem final : public smile::ecs::System
    {
      public:
        TransformSystem( smile::ecs::ECSEngine *pECSEngine, Scene *pScene );

        void OnUpdate( primitive::Timestep deltaTime ) override;

      private:
        Scene *m_pScene;
    };
}