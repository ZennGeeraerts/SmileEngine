#pragma once

namespace Smile
{
    class Window;
}

namespace Smile::Graphic
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