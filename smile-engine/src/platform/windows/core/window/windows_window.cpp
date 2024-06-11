/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "windows_window.h"
#include "smile_engine/common/logger/logger.h"

#include "smile_engine/core/window/events/application_event.h"
#include "smile_engine/core/window/events/mouse_event.h"
#include "smile_engine/core/window/events/key_event.h"
#include "smile_engine/core/input/input.h"

#include "smile_engine/graphic/graphics_context.h"

#include "windows_window_manager.h"

//#include "SmileEngine/../../resource.h"

//#include "SmileEngine/ImGui/imgui_impl_win32.h"
//
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace smile::window
{
    WindowsWindow::WindowsWindow( const WindowSettings &settings, const std::string &className )
    {
        Initialize( settings, className );
    }

    WindowsWindow::~WindowsWindow()
    {
        ShutDown();
    }

    void WindowsWindow::ShutDown()
    {
        DestroyWindow( m_WindowHandle );
        
        delete m_pContext;
        delete m_pDevice;
    }

    void WindowsWindow::Initialize( const WindowSettings &settings, const std::string &className )
    {
        m_Data.Settings = settings;

        SM_LOG_INFO( "WindowsWindow::Initialize > Creating window: %s (%d, %d)",
            settings.Title.c_str(),
            settings.Width,
            settings.Height );

        RECT windowRect{};
        windowRect.left = 0;
        windowRect.right = settings.Width + windowRect.left;
        windowRect.top = 0;
        windowRect.bottom = settings.Height + windowRect.top;
        AdjustWindowRect(
            &windowRect, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE );

        // Create and display the window
        auto windowTitle = std::wstring{ settings.Title.begin(), settings.Title.end() };
        auto classNameWStr = std::wstring{ className.begin(), className.end() };
        m_WindowHandle = CreateWindow( classNameWStr.c_str(),
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

        SM_ASSERT( m_WindowHandle, "WindowsWindow::Initialize > Could not create window!" );

        // Init device and context
        m_pDevice = graphic::GraphicsDevice::Create();
        m_pContext = graphic::GraphicsContext::Create( this );

        m_pDevice->Initialize( m_pContext );
        m_pContext->Initialize( m_pDevice );

        ShowWindow( m_WindowHandle, SW_SHOW );
        UpdateWindow( m_WindowHandle );
        SM_LOG_INFO( "WindowsWindow::Initialize > Window '%s' created", settings.Title.c_str() );

        SetVSync( true );
        m_IsInitialized = true;
    }

    void WindowsWindow::OnUpdate()
    {
        if ( !m_IsInitialized )
            return;

        m_pContext->Present();
    }

    LRESULT WindowsWindow::WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
    {
        if ( !m_IsInitialized )
            return DefWindowProc( hWnd, msg, wParam, lParam );

        /*if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;*/

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
                DestroyWindow( m_WindowHandle );
                break;
            }

            case WM_DESTROY:
            {
                PostQuitMessage( 0 );
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
