/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "windows_window_manager.h"

#include "smile/common/memory/scope.h"
#include "windows_window.h"

namespace smile::window
{
    static memory::Scope<WindowsWindowManager> s_WindowManager{ nullptr };

    WindowManager *WindowManager::Create()
    {
        SM_ASSERT( !s_WindowManager );

        s_WindowManager = memory::CreateScope< WindowsWindowManager >();

        return s_WindowManager.GetPointer();
    }

    WindowsWindowManager::WindowsWindowManager()
    {
        // Create window class
        m_WindowClass = {};
        m_WindowClass.cbSize = sizeof( WNDCLASSEX );
        m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
        m_WindowClass.cbClsExtra = 0;
        m_WindowClass.cbWndExtra = 0;

        m_WindowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
        m_WindowClass.hbrBackground = ( HBRUSH )GetStockObject( NULL_BRUSH );

        m_WindowClass.hIcon = static_cast< HICON >( ::LoadImage(
            NULL, L"resources/icons/logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED ) );
        m_WindowClass.hIconSm = m_WindowClass.hIcon;

        m_WindowClass.lpszClassName = WindowsWindow::ClassName;
        m_WindowClass.lpszMenuName = nullptr;

        m_WindowClass.hInstance = HINSTANCE();
        m_WindowClass.lpfnWndProc = WindowsProcedureStatic;

        int success = RegisterClassEx( &m_WindowClass );
        SM_ASSERT_MSG( success, "WindowsWindowManager > Could not register window class!" );
    }

    WindowsWindowManager::~WindowsWindowManager()
    {
        UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
    }

    Window::Ref WindowsWindowManager::CreateNewWindow( const WindowSettings &windowSettings )
    {
        auto pWindow = memory::CreateRef< WindowsWindow >( windowSettings );

        m_pWindows.PushBack( pWindow );
        pWindow->Initialize();

        return pWindow;
    }

    void WindowsWindowManager::PollEvents()
    {
        // If there are window messages then process them
        MSG message{};
        if ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &message );
            DispatchMessage( &message );
        }
    }

    LRESULT CALLBACK WindowsWindowManager::WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        return s_WindowManager->WindowsProcedure( hWnd, msg, wParam, lParam );
    }

    LRESULT WindowsWindowManager::WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        if ( msg == WM_NCCREATE )
        {
            const CREATESTRUCTW *const pCreate = reinterpret_cast< CREATESTRUCTW * >( lParam );
            SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pCreate->lpCreateParams ) );

            return 0;
        }
        else if ( msg == WM_DESTROY )
        {
            WindowsWindow *const pWindow =
                reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

            if ( pWindow )
            {
                s_WindowManager->m_pWindows.Erase( pWindow );
            }

            PostQuitMessage( 0 );
            
            return 0;
        }

        WindowsWindow *const pWindow = reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

        if ( pWindow )
        {
            return pWindow->WindowsProcedure( hWnd, msg, wParam, lParam );
        }

        return DefWindowProc( hWnd, msg, wParam, lParam );
    }
}