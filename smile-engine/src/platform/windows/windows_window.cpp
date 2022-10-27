#include "smpch.h"
#include "windows_window.h"
#include "smile_engine/core/logger.h"

#include "smile_engine/core/events/application_event.h"
#include "smile_engine/core/events/mouse_event.h"
#include "smile_engine/core/events/key_event.h"

#include "smile_engine/graphic/graphics_context.h"
#include "smile_engine/graphic/renderer.h"
#include "smile_engine/input/input.h"

//#include "SmileEngine/../../resource.h"

//#include "SmileEngine/ImGui/imgui_impl_win32.h"
//
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace smile
{
    Window *Window::create( const WindowSettings &settings )
    {
        return new WindowsWindow{ settings };
    }

    WindowsWindow::WindowsWindow( const WindowSettings &settings )
    {
        init( settings );
    }

    WindowsWindow::~WindowsWindow()
    {
        shutDown();
    }

    void WindowsWindow::shutDown()
    {
        DestroyWindow( windowHandle );
        UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );
        delete context;
    }

    void WindowsWindow::init( const WindowSettings &settings )
    {
        data.title = settings.title;
        data.height = settings.height;
        data.width = settings.width;

        message = { 0 };

        SM_LOG_INFO( "WindowsWindow::init > Creating window: %s (%d, %d)",
            settings.title.c_str(),
            settings.width,
            settings.height );

        // Create window class
        const wchar_t *class_name = L"SmileWindowClass";
        windowClass = {};
        windowClass.cbSize = sizeof( WNDCLASSEX );
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;

        windowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
        windowClass.hbrBackground = ( HBRUSH )GetStockObject( NULL_BRUSH );

       windowClass.hIcon = static_cast< HICON >( ::LoadImage(
            NULL, L"assets/icons/logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED ) );
        windowClass.hIconSm = windowClass.hIcon;

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

        windowClass.lpszClassName = class_name;
        windowClass.lpszMenuName = nullptr;

        windowClass.hInstance = HINSTANCE();
        windowClass.lpfnWndProc = windowsProcedureStatic;

        int success = RegisterClassEx( &windowClass );
        SM_ASSERT( success, "Could not register window class!" );

        RECT window_rect{};
        window_rect.left = 0;
        window_rect.right = settings.width + window_rect.left;
        window_rect.top = 0;
        window_rect.bottom = settings.height + window_rect.top;
        AdjustWindowRect(
            &window_rect, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE );

        // Create and display the window
        auto window_title = std::wstring{ settings.title.begin(), settings.title.end() };
        windowHandle = CreateWindow( class_name,
            window_title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            nullptr,
            nullptr,
            HINSTANCE(),
            this );

        SM_ASSERT( windowHandle, "WindowsWindow::init > Could not create window!" );

        // Init context
        context = graphic::GraphicsContext::create( this );
        context->init();

        ShowWindow( windowHandle, SW_SHOW );
        UpdateWindow( windowHandle );
        SM_LOG_INFO( "WindowsWindow::init > Window '%s' created", settings.title.c_str() );

        setVSync( true );
        initialized = true;
    }

    void WindowsWindow::onUpdate()
    {
        if ( !initialized )
            return;

        pollEvents();
        context->present();
    }

    void WindowsWindow::pollEvents()
    {
        if ( message.message != WM_QUIT )
        {
            // If there are window messages then process them
            if ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
            {
                TranslateMessage( &message );
                DispatchMessage( &message );
            }
        }
    }

    void WindowsWindow::setVSync( bool enabled )
    {
        data.vsync = enabled;
    }

    bool WindowsWindow::isVSync() const
    {
        return data.vsync;
    }

    LRESULT CALLBACK WindowsWindow::windowsProcedureStatic( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param )
    {
        if ( msg == WM_NCCREATE )
        {
            const CREATESTRUCTW *const create = reinterpret_cast< CREATESTRUCTW * >( l_param );
            SetWindowLongPtr( h_wnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( create->lpCreateParams ) );
        }
        else
        {
            WindowsWindow *const window =
                reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( h_wnd, GWLP_USERDATA ) );
            if ( window )
                return window->windowsProcedure( h_wnd, msg, w_param, l_param );
        }
        return DefWindowProc( h_wnd, msg, w_param, l_param );
    }

    LRESULT WindowsWindow::windowsProcedure( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) noexcept
    {
        if ( !initialized )
            return DefWindowProc( h_wnd, msg, w_param, l_param );

        /*if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;*/

        WindowsWindow *const window = reinterpret_cast< WindowsWindow * >( GetWindowLongPtr( h_wnd, GWLP_USERDATA ) );

        switch ( msg )
        {
            case WM_ACTIVATE:
            {
                return 0;
            }

            case WM_CLOSE:
            {
                WindowCloseEvent event{};
                window->data.eventCallback( event );
                DestroyWindow( windowHandle );
                break;
            }

            case WM_DESTROY:
            {
                PostQuitMessage( 0 );
                break;
            }

            case WM_SIZE:
            {
                Uint32 width = LOWORD( l_param );
                Uint32 height = HIWORD( l_param );
                WindowResizeEvent event{ width, height };
                window->data.eventCallback( event );
                window->data.width = width;
                window->data.height = height;
                break;
            }

            case WM_KEYDOWN:
            {
                KeyPressedEvent event{ static_cast< input::KeyCode >( w_param ), static_cast< Uint16 >( l_param & 0xFF ) };
                window->data.eventCallback( event );
                break;
            }

            case WM_KEYUP:
            {
                KeyReleasedEvent event{ static_cast< input::KeyCode >( w_param ) };
                window->data.eventCallback( event );
                break;
            }

            case WM_LBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 0 };
                window->data.eventCallback( event );
                break;
            }

            case WM_RBUTTONDOWN:
            {
                MouseButtonPressedEvent event{ 1 };
                window->data.eventCallback( event );
                break;
            }

            case WM_LBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 0 };
                window->data.eventCallback( event );
                break;
            }

            case WM_RBUTTONUP:
            {
                MouseButtonReleasedEvent event{ 1 };
                window->data.eventCallback( event );
                break;
            }

            case WM_MOUSEWHEEL:
            {
                float z_delta = GET_WHEEL_DELTA_WPARAM( w_param );
                MouseScrolledEvent event{ z_delta, 0 };
                window->data.eventCallback( event );
                break;
            }

            case WM_MOUSEMOVE:
            {
                float x_pos = static_cast< float >( GET_X_LPARAM( l_param ) );
                float y_pos = static_cast< float >( GET_Y_LPARAM( l_param ) );
                MouseMovedEvent event{ x_pos, y_pos };
                window->data.eventCallback( event );
                break;
            }

            case WM_CHAR:
            {
                KeyTypedEvent event{ static_cast< input::KeyCode >( w_param ) };
                window->data.eventCallback( event );
                break;
            }

            default:
                return DefWindowProc( h_wnd, msg, w_param, l_param );
        }

        return 0;
    }
}
