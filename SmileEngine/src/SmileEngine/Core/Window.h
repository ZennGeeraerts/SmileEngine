#pragma once
#include "smpch.h"
#include "SmileEngine/Core/Core.h"
#include "SmileEngine/Events/Event.h"

namespace smile
{
    struct WindowSettings
    {
        WindowSettings( const std::string &title = "Smile Engine", Uint32 width = 1280, Uint32 height = 720 )
            : m_Title{ title }, m_Width{ width }, m_Height{ height }
        {
        }

        std::string m_Title;
        Uint32 m_Width;
        Uint32 m_Height;
    };

    class GraphicsContext;

    // Window interface for desktop platforms
    class Window
    {
      public:
        using EventCallbackFunction = std::function< void( Event & ) >;

        virtual ~Window()
        {
        }

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual GraphicsContext *GetGraphicsContext() const = 0;

        // Window attributes
        virtual void SetEventCallback( const EventCallbackFunction &callback ) = 0;
        virtual void SetVSync( bool bEnabled ) = 0;
        virtual bool IsVSync() const = 0;

        // Returns the child window
        virtual void *GetNativeWindow() const = 0;

        static Window *Create( const WindowSettings &settings = WindowSettings{} );
    };
}
