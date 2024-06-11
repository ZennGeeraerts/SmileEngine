#pragma once
#include "smile_engine/core/ecs/system.h"

namespace smile::scene
{
    class Scene;

    class TransformSystem final : public ecs::System
    {
      public:
        TransformSystem( ecs::ECSEngine *pECSEngine, Scene *pScene );

        void OnUpdate( primitive::Timestep deltaTime ) override;

      private:
        Scene *m_pScene;
    };
}