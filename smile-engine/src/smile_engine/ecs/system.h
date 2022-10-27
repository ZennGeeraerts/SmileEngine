#pragma once

#include "ecs_engine.h"

#include "smile_engine/core/timestep.h"

namespace smile::ecs
{
	class System final
    {
      public:
        System() = default;

        virtual void onUpdate( Timestep delta_time ) = 0;
    };
}