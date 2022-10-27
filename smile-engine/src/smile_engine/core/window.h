#pragma once
#include "smpch.h"
#include "smile_engine/core/core.h"
#include "smile_engine/core/events/event.h"

namespace smile
{
    struct WindowSettings
    {
        WindowSettings( const std::string &title = "Smile Engine", Uint32 width = 1280, Uint32 height = 720 )
            : title{ title }, width{ width }, height{ height }
        {
        }

        std::string title;
        Uint32 width;
        Uint32 height;
    };

    namespace graphic
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

        virtual void onUpdate() = 0;

        virtual Uint32 getWidth() const = 0;
        virtual Uint32 getHeight() const = 0;
        virtual graphic::GraphicsContext *getGraphicsContext() const = 0;

        // Window attributes
        virtual void setEventCallback( const EventCallbackFunction &callback ) = 0;
        virtual void setVSync( bool enabled ) = 0;
        virtual bool isVSync() const = 0;

        // Returns the child window
        virtual void *getNativeWindow() const = 0;

        static Window *create( const WindowSettings &settings = WindowSettings{} );
    };
}
