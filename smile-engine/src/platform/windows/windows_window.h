#pragma once

#include "smile_engine/core/window.h"
#include "smile_engine/graphic/graphics_context.h"

#define HINSTANCE() GetModuleHandle( NULL ) // this function returns the hInstance

namespace smile
{
    class WindowsWindow final : public Window
    {
     public:
        WindowsWindow( const WindowSettings &settings, const std::string &className );
        virtual ~WindowsWindow();

        WindowsWindow( const WindowsWindow & ) = delete;
        WindowsWindow( WindowsWindow && ) = delete;
        WindowsWindow &operator=( const WindowsWindow & ) = delete;
        WindowsWindow &operator=( WindowsWindow && ) = delete;

        void OnUpdate() override;

        inline virtual void *GetNativeWindow() const override
        {
            return m_WindowHandle;
        }

        LRESULT WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

      private:
        void Initialize( const WindowSettings &settings, const std::string &className );
        void ShutDown();

      private:
        HWND m_WindowHandle;
    };
}
