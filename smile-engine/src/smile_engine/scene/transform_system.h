#pragma once
#include "smile_engine/ecs/system.h"

namespace smile::scene
{
    class Scene;

    class TransformSystem final : public ecs::System
    {
      public:
        TransformSystem( ecs::ECSEngine *pECSEngine, Scene *pScene );

        void OnUpdate( Timestep deltaTime ) override;

      private:
        Scene *m_pScene;
    };
}