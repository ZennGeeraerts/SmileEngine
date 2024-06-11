/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "windows_window_manager.h"

#include "windows_window.h"

namespace smile::window
{
    WindowManager* WindowManager::Create()
    {
        return new WindowsWindowManager{};
    }

    WindowsWindowManager::WindowsWindowManager()
    {
        // Create window class
        const wchar_t *className = L"SmileWindowClass";
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

        /*if ( !m_WindowClass.hIcon )
        {
            DWORD dLastError = GetLastError();

            LPCTSTR strErrorMessage = NULL;

            FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY |
                               FORMAT_MESSAGE_ALLOCATE_BUFFER,
                NULL,
                dLastError,
                0,
                ( LPWSTR )&strErrorMessage,
                0,
                NULL );

            MessageBox( m_WindowHandle, strErrorMessage, L"Error", MB_OK );
        }*/

        m_WindowClass.lpszClassName = className;
        m_WindowClass.lpszMenuName = nullptr;

        m_WindowClass.hInstance = HINSTANCE();
        m_WindowClass.lpfnWndProc = WindowsProcedureStatic;

        int success = RegisterClassEx( &m_WindowClass );
        SM_ASSERT( success, "WindowsWindowManager > Could not register window class!" );
    }

    WindowsWindowManager::~WindowsWindowManager()
    {
        UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
    }

    Window *WindowsWindowManager::CreateNewWindow( const WindowSettings &windowSettings )
    {
        std::wstring classNameWStr{ m_WindowClass.lpszClassName };
        std::string className{ classNameWStr.begin(), classNameWStr.end() };

        auto pWindow = new WindowsWindow{ windowSettings, className };
        m_pWindows.push_back( pWindow );
        return pWindow;
    }

    void WindowsWindowManager::OnUpdate()
    {
        PollEvents();

        for ( const auto &pWindow : m_pWindows )
            pWindow->OnUpdate();
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
        if ( msg == WM_NCCREATE )
        {
            const CREATESTRUCTW *const pCreate = reinterpret_cast< CREATESTRUCTW * >( lParam );
            SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pCreate->lpCreateParams ) );
        }
        else
        {
            WindowsWindow *const pWindow =
                reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            if ( pWindow )
                return pWindow->WindowsProcedure( hWnd, msg, wParam, lParam );
        }
        return DefWindowProc( hWnd, msg, wParam, lParam );
    }
}