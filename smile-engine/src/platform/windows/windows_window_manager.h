#pragma once
#include "smile_engine/core/window_manager.h"

namespace smile
{
	class WindowsWindowManager final : public WindowManager
    {
      public:
        WindowsWindowManager();
        virtual ~WindowsWindowManager();

        Window *CreateNewWindow( const WindowSettings &windowSettings ) override;
        void OnUpdate() override;

      private:
        void PollEvents();

        static LRESULT CALLBACK WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

      private:
        WNDCLASSEX m_WindowClass;
	};
}