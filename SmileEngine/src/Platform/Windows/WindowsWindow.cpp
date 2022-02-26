#include "smpch.h"
#include "WindowsWindow.h"
#include "SmileEngine/Core/Logger.h"

#include "SmileEngine/Events/ApplicationEvent.h"
#include "SmileEngine/Events/MouseEvent.h"
#include "SmileEngine/Events/KeyEvent.h"

#include "SmileEngine/Renderer/GraphicsContext.h"
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Input/Input.h"

//#include "SmileEngine/../../resource.h"

//#include "SmileEngine/ImGui/imgui_impl_win32.h"
//
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace smile
{
    Window *Window::Create( const WindowSettings &settings )
    {
        return new WindowsWindow{ settings };
    }

    WindowsWindow::WindowsWindow( const WindowSettings &settings )
    {
        Init( settings );
    }

    WindowsWindow::~WindowsWindow()
    {
        ShutDown();
    }

    void WindowsWindow::ShutDown()
    {
        DestroyWindow( m_WindowHandle );
        UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
        delete m_pContext;
    }

    void WindowsWindow::Init( const WindowSettings &settings )
    {
        m_Data.m_Title = settings.m_Title;
        m_Data.m_Height = settings.m_Height;
        m_Data.m_Width = settings.m_Width;

        m_Message = { 0 };

        SM_LOG_INFO( "WindowsWindow::Init > Creating window: %s (%d, %d)",
            settings.m_Title.c_str(),
            settings.m_Width,
            settings.m_Height );

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
            NULL, L"Resources/Icons/logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED ) );
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
        SM_ASSERT( success, "Could not register window class!" );

        RECT windowRect{};
        windowRect.left = 0;
        windowRect.right = settings.m_Width + windowRect.left;
        windowRect.top = 0;
        windowRect.bottom = settings.m_Height + windowRect.top;
        AdjustWindowRect(
            &windowRect, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE );

        // Create and display the window
        auto windowTitle = std::wstring{ settings.m_Title.begin(), settings.m_Title.end() };
        m_WindowHandle = CreateWindow( className,
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

        SM_ASSERT( m_WindowHandle, "WindowsWindow::Init > Could not create window!" );

        // Init context
        m_pContext = GraphicsContext::Create( this );
        m_pContext->Init();

        ShowWindow( m_WindowHandle, SW_SHOW );
        UpdateWindow( m_WindowHandle );
        SM_LOG_INFO( "WindowsWindow::Init > Window '%s' created", settings.m_Title.c_str() );

        SetVSync( true );
        m_bInitialized = true;
    }

    void WindowsWindow::OnUpdate()
    {
        if ( !m_bInitialized )
            return;

        PollEvents();
        m_pContext->Present();
    }

    void WindowsWindow::PollEvents()
    {
        if ( m_Message.message != WM_QUIT )
        {
            // If there are window messages then process them
            if ( PeekMessage( &m_Message, 0, 0, 0, PM_REMOVE ) )
            {
                TranslateMessage( &m_Message );
                DispatchMessage( &m_Message );
            }
        }
    }

    void WindowsWindow::SetVSync( bool bEnabled )
    {
        m_Data.m_bVSync = bEnabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.m_bVSync;
    }

    LRESULT CALLBACK WindowsWindow::WindowsProcedureStatic( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        if ( msg == WM_NCCREATE )
        {
            const CREATESTRUCTW *const pCreate = reinterpret_cast< CREATESTRUCT * >( lParam );
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

    LRESULT WindowsWindow::WindowsProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
    {
        if ( !m_bInitialized )
            return DefWindowProc( hWnd, msg, wParam, lParam );

        /*if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;*/

        WindowsWindow *const pWindow = reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

        switch ( msg )
        {
            case WM_ACTIVATE:
            {
                return 0;
            }

            case WM_CLOSE:
            {
                WindowCloseEvent event{};
                pWindow->m_Data.m_EventCallback( event );
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
                unsigned int width = LOWORD( lParam );
                unsigned int height = HIWORD( lParam );
                WindowResizeEvent event{ width, height };
                pWindow->m_Data.m_EventCallback( event );
                pWindow->m_Data.m_Width = width;
                pWindow->m_Data.m_Height = height;
                break;
            }

            case WM_KEYDOWN:
            {
                KeyPressedEvent event{ static_cast< KeyCode >( wParam ), static_cast< Uint16 >( lParam & 0xFF ) };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_KEYUP:
            {
                KeyReleasedEvent event{ static_cast< KeyCode >( wParam ) };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_LBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 0 };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_RBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 1 };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_LBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 0 };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_RBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 1 };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_MOUSEWHEEL:
            {
                float zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
                MouseScrolledEvent event{ zDelta, 0 };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_MOUSEMOVE:
            {
                float xPos = static_cast< float >( GET_X_LPARAM( lParam ) );
                float yPos = static_cast< float >( GET_Y_LPARAM( lParam ) );
                MouseMovedEvent event{ xPos, yPos };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            case WM_CHAR:
            {
                KeyTypedEvent event{ static_cast< KeyCode >( wParam ) };
                pWindow->m_Data.m_EventCallback( event );
                break;
            }

            default:
                return DefWindowProc( hWnd, msg, wParam, lParam );
        }

        return 0;
    }
}
