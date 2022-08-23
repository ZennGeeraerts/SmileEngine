#pragma once

namespace smile::ecs
{
	class System final
    {
      public:
        System() = default;

        virtual void update() = 0;
    };
}