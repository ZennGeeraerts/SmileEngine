#pragma once

namespace smile
{
    class Window;

    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void Present() = 0;

        static GraphicsContext *Create( Window *pWindow );
    };
}