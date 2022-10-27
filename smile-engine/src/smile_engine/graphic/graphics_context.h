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
        virtual void init() = 0;
        virtual void present() = 0;

        static GraphicsContext *create( Window *window );
    };
}