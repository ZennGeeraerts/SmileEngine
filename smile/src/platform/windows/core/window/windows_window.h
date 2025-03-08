/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/window/window.h"

#include <Windows.h>

#define HINSTANCE() GetModuleHandle( NULL ) // this function returns the hInstance

namespace smile::window
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
