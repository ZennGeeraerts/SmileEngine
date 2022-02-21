#pragma once

namespace smile
{
    class Window;

    class RenderingContext
    {
      public:
        virtual ~RenderingContext() = default;
        virtual void Init() = 0;
        virtual void Present() = 0;

        static RenderingContext *Create( Window *pWindow );
    };
}