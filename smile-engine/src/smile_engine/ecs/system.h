#pragma once

#include "ecs_engine.h"

#include "smile_engine/core/timestep.h"

namespace Smile::ECS
{
	class System final
    {
      public:
        System() = default;

        virtual void onUpdate( Timestep delta_time ) = 0;
    };
}