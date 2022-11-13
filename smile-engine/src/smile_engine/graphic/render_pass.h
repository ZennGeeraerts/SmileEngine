#pragma once

namespace smile::graphic
{
    class RenderPass
    {
      public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        virtual void OnRender() = 0;
    };
}