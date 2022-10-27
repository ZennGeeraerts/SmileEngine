#pragma once

#include <thirdparty/entt/entt.hpp>

namespace smile::graphic
{
    class RenderPass
    {
      public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        virtual void addToScene( entt::registry registry ) = 0;
        virtual void onRender() = 0;
    };
}