/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/window/window.h"

#include <Windows.h>

namespace smile::window
{
    class WindowsWindow final : public Window
    {
      public:
        WindowsWindow( const WindowSettings &settings );
        ~WindowsWindow() = default;

        WindowsWindow( const WindowsWindow & ) = delete;
        WindowsWindow( WindowsWindow && ) = delete;
        WindowsWindow &operator=( const WindowsWindow & ) = delete;
        WindowsWindow &operator=( WindowsWindow && ) = delete;

        void Initialize() override;
        void ShutDown() override;

        inline virtual void *GetNativeWindow() const override
        {
            return m_WindowHandle;
        }

        LRESULT WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

        static constexpr const wchar_t *ClassName{ L"SmileWindowClass" };

      private:
        HWND m_WindowHandle;
    };
}
