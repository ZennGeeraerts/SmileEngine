#pragma once

#include "smile_engine/core/window.h"
#include "smile_engine/graphic/graphics_context.h"

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

        void OnUpdate() override;

        inline Uint32 GetWidth() const override
        {
            return m_Data.Width;
        }
        inline Uint32 GetHeight() const override
        {
            return m_Data.Height;
        }
        inline graphic::GraphicsDevice* GetGraphicsDevice() const override
        {
            return m_pDevice;
        }
        inline graphic::GraphicsContext *GetGraphicsContext() const override
        {
            return m_pContext;
        }

        // Window attributes
        inline void SetEventCallback( const EventCallbackFunction &callback ) override
        {
            m_Data.EventCallback = callback;
        }
        virtual void SetVSync( bool isEnabled ) override;
        virtual bool IsVSync() const override;

        inline virtual void *GetNativeWindow() const override
        {
            return m_WindowHandle;
        }

      private:
        virtual void Initialize( const WindowSettings &settings );
        virtual void ShutDown();

        static LRESULT CALLBACK WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
        LRESULT WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

        void PollEvents();

      private:
        HWND m_WindowHandle;
        WNDCLASSEX m_WindowClass;
        MSG m_Message;
        graphic::GraphicsDevice *m_pDevice;
        graphic::GraphicsContext *m_pContext;

        struct WindowData
        {
            std::string Title{};
            Uint32 Width{};
            Uint32 Height{};
            bool IsVSync{};
            EventCallbackFunction EventCallback{};
        };

        WindowData m_Data;
        bool m_IsInitialized = false;
    };
}
