#pragma once

#include "smile_engine/core/window.h"
#include "smile_engine/renderer/graphics_context.h"

#define HINSTANCE() GetModuleHandle( NULL ) // this function returns the hInstance

namespace smile
{
    class WindowsWindow final : public Window
    {
     public:
        WindowsWindow( const WindowSettings &settings );
        virtual ~WindowsWindow();

        WindowsWindow( const WindowsWindow & ) = delete;
        WindowsWindow( WindowsWindow && ) = delete;
        WindowsWindow &operator=( const WindowsWindow & ) = delete;
        WindowsWindow &operator=( WindowsWindow && ) = delete;

        void onUpdate() override;

        inline Uint32 getWidth() const override
        {
            return data.width;
        }
        inline Uint32 getHeight() const override
        {
            return data.height;
        }
        inline renderer::GraphicsContext *getGraphicsContext() const override
        {
            return context;
        }

        // Window attributes
        inline void setEventCallback( const EventCallbackFunction &callback ) override
        {
            data.eventCallback = callback;
        }
        virtual void setVSync( bool enabled ) override;
        virtual bool isVSync() const override;

        inline virtual void *getNativeWindow() const override
        {
            return windowHandle;
        }

      private:
        virtual void init( const WindowSettings &settings );
        virtual void shutDown();

        static LRESULT CALLBACK windowsProcedureStatic( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param );
        LRESULT windowsProcedure( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) noexcept;

        void pollEvents();

      private:
        HWND windowHandle;
        WNDCLASSEX windowClass;
        MSG message;
        renderer::GraphicsContext *context;

        struct WindowData
        {
            std::string title{};
            Uint32 width{};
            Uint32 height{};
            bool vsync{};
            EventCallbackFunction eventCallback{};
        };

        WindowData data;
        bool initialized = false;
    };
}
