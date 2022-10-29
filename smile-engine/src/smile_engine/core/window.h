#pragma once
#include "smpch.h"
#include "smile_engine/core/core.h"
#include "smile_engine/core/events/event.h"

namespace Smile
{
    struct WindowSettings
    {
        WindowSettings( const std::string &title = "Smile Engine", Uint32 width = 1280, Uint32 height = 720 )
            : Title{ title }, Width{ width }, Height{ height }
        {
        }

        std::string Title;
        Uint32 Width;
        Uint32 Height;
    };

    namespace Graphic
    {
        class GraphicsContext;
    }

    // Window interface for desktop platforms
    class Window
    {
      public:
        using EventCallbackFunction = std::function< void( Event & ) >;

        virtual ~Window()
        {
        }

        virtual void OnUpdate() = 0;

        virtual Uint32 GetWidth() const = 0;
        virtual Uint32 GetHeight() const = 0;
        virtual Graphic::GraphicsContext *GetGraphicsContext() const = 0;

        // Window attributes
        virtual void SetEventCallback( const EventCallbackFunction &callback ) = 0;
        virtual void SetVSync( bool isEnabled ) = 0;
        virtual bool IsVSync() const = 0;

        // Returns the child window
        virtual void *GetNativeWindow() const = 0;

        static Window *Create( const WindowSettings &settings = WindowSettings{} );
    };
}
