/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "windows_window.h"

#include "smile/common/logging/logging.h"

#include "smile/core/window/events/application_event.h"
#include "smile/core/window/events/mouse_event.h"
#include "smile/core/window/events/key_event.h"
#include "smile/core/input/input.h"

#include "windows_window_manager.h"

#include <windowsx.h>

namespace smile::window
{
    WindowsWindow::WindowsWindow( const WindowSettings &settings ) : m_WindowHandle{ nullptr }
    {
        m_Data.Settings = settings;
    }

    void WindowsWindow::Initialize()
    {
        SM_LOG_INFO( "WindowsWindow::Initialize > Creating window: {0} ({1}, {2})",
            m_Data.Settings.Title,
            m_Data.Settings.Width,
            m_Data.Settings.Height );

        RECT windowRect{};
        windowRect.left = 0;
        windowRect.right = m_Data.Settings.Width + windowRect.left;
        windowRect.top = 0;
        windowRect.bottom = m_Data.Settings.Height + windowRect.top;
        AdjustWindowRect(
            &windowRect, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE );

        // Create and display the window
        const int size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, m_Data.Settings.Title.GetData(), -1, nullptr, 0 );
        SM_ASSERT( size != 0 );
        std::wstring windowTitle;
        windowTitle.resize( size );
        MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, m_Data.Settings.Title.GetData(), -1, windowTitle.data(), size );

        m_WindowHandle = CreateWindow( ClassName,
            windowTitle.c_str(),
            WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            HINSTANCE(),
            this );

        SM_ASSERT_MSG( m_WindowHandle, "WindowsWindow::Initialize > Could not create window!" );

        ShowWindow( m_WindowHandle, SW_SHOW );
        UpdateWindow( m_WindowHandle );
        SM_LOG_INFO( "WindowsWindow::Initialize > Window '{}' created", m_Data.Settings.Title );

        SetVSync( true );
        m_IsInitialized = true;
    }

    void WindowsWindow::ShutDown()
    {
        DestroyWindow( m_WindowHandle );
    }

    LRESULT WindowsWindow::WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
    {
        if ( !m_IsInitialized )
            return DefWindowProc( hWnd, msg, wParam, lParam );

        switch ( msg )
        {
            case WM_ACTIVATE:
            {
                return 0;
            }

            case WM_CLOSE:
            {
                WindowCloseEvent event{};
                m_Data.EventCallback( event );
                ShutDown();
                break;
            }

            case WM_SIZE:
            {
                Uint32 width = LOWORD( lParam );
                Uint32 height = HIWORD( lParam );
                WindowResizeEvent event{ width, height };
                m_Data.EventCallback( event );
                m_Data.Settings.Width = width;
                m_Data.Settings.Height = height;
                break;
            }

            case WM_KEYDOWN:
            {
                KeyPressedEvent event{
                    static_cast< input::KeyCode >( wParam ), static_cast< Uint16 >( lParam & 0xFF ) };
                m_Data.EventCallback( event );
                break;
            }

            case WM_KEYUP:
            {
                KeyReleasedEvent event{ static_cast< input::KeyCode >( wParam ) };
                m_Data.EventCallback( event );
                break;
            }

            case WM_LBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 0 };
                m_Data.EventCallback( event );
                break;
            }

            case WM_RBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 1 };
                m_Data.EventCallback( event );
                break;
            }

            case WM_LBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 0 };
                m_Data.EventCallback( event );
                break;
            }

            case WM_RBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 1 };
                m_Data.EventCallback( event );
                break;
            }

            case WM_MOUSEWHEEL:
            {
                float zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
                MouseScrolledEvent event{ zDelta, 0 };
                m_Data.EventCallback( event );
                break;
            }

            case WM_MOUSEMOVE:
            {
                float xPos = static_cast< float >( GET_X_LPARAM( lParam ) );
                float yPos = static_cast< float >( GET_Y_LPARAM( lParam ) );
                MouseMovedEvent event{ xPos, yPos };
                m_Data.EventCallback( event );
                break;
            }

            case WM_CHAR:
            {
                KeyTypedEvent event{ static_cast< input::KeyCode >( wParam ) };
                m_Data.EventCallback( event );
                break;
            }

            default:
                return DefWindowProc( hWnd, msg, wParam, lParam );
        }

        return 0;
    }
}
