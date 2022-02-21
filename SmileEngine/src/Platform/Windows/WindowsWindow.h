#pragma once

#include "SmileEngine/Core/Window.h"
#include "SmileEngine/Renderer/GraphicsContext.h"

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

        inline unsigned int GetWidth() const override
        {
            return m_Data.m_Width;
        }
        inline unsigned int GetHeight() const override
        {
            return m_Data.m_Height;
        }
        inline GraphicsContext *GetGraphicsContext() const override
        {
            return m_pContext;
        }

        // Window attributes
        inline void SetEventCallback( const EventCallbackFunction &callback ) override
        {
            m_Data.m_EventCallback = callback;
        }
        virtual void SetVSync( bool bEnabled ) override;
        virtual bool IsVSync() const override;

        inline virtual void *GetNativeWindow() const override
        {
            return m_WindowHandle;
        }

      private:
        virtual void Init( const WindowSettings &settings );
        virtual void ShutDown();

        static LRESULT CALLBACK WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
        LRESULT WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

        void PollEvents();

      private:
        HWND m_WindowHandle;
        WNDCLASSEX m_WindowClass;
        MSG m_Message;
        GraphicsContext *m_pContext;

        struct WindowData
        {
            std::string m_Title{};
            uint32_t m_Width{};
            uint32_t m_Height{};
            bool m_bVSync{};
            EventCallbackFunction m_EventCallback{};
        };

        WindowData m_Data;
        bool m_bInitialized = false;
    };
}
