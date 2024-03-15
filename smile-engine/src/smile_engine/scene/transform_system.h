#pragma once
#include "smile_engine/ecs/system.h"

namespace smile::scene
{
    class TransformSystem final : public ecs::System
    {
      public:
        TransformSystem( ecs::ECSEngine *pECSEngine );

        void OnUpdate( Timestep deltaTime ) override;
    };
}