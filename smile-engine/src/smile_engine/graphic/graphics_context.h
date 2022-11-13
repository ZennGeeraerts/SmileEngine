#pragma once

namespace smile
{
    class Window;
}

namespace smile::graphic
{
    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;
        virtual void Initialize() = 0;
        virtual void Present() = 0;

        static GraphicsContext *Create( Window *pWindow );
    };
}