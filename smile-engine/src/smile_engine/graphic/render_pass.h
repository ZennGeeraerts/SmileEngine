#pragma once

namespace Smile::Graphic
{
    class RenderPass
    {
      public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        virtual void OnRender() = 0;
    };
}