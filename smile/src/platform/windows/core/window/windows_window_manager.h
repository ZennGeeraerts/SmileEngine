/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/core/window/window_manager.h"

#include <Windows.h>

namespace smile::window
{
	class WindowsWindowManager final : public WindowManager
    {
      public:
        WindowsWindowManager();
        virtual ~WindowsWindowManager();

        Window *CreateNewWindow( const WindowSettings &windowSettings ) override;
        void PollEvents() override;

      private:
        static LRESULT CALLBACK WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

      private:
        WNDCLASSEX m_WindowClass;
	};
}