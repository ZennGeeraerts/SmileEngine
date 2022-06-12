#pragma once

namespace smile
{
    class Window;
}

namespace smile::renderer
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