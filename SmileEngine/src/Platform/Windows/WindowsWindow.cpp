#include "smpch.h"
#include "WindowsWindow.h"
#include "SmileEngine/Logger.h"

#include "SmileEngine/Events/ApplicationEvent.h"
#include "SmileEngine/Events/MouseEvent.h"
#include "SmileEngine/Events/KeyEvent.h"

namespace Smile
{
	Window* Window::Create(const WindowSettings& settings)
	{
		return new WindowsWindow(settings);
	}

	WindowsWindow::WindowsWindow(const WindowSettings& settings)
	{
		Init(settings);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::ShutDown()
	{
		DestroyWindow(m_WindowHandle);
	}

	void WindowsWindow::Init(const WindowSettings& settings)
	{
		m_Data.Title = settings.Title;
		m_Data.Height = settings.Height;
		m_Data.Width = settings.Width;

		SM_INFO("Creating window: %s (%d, %d)", settings.Title.c_str(), settings.Width, settings.Height);

		// Create window class

		const wchar_t* className = L"SmileWindowClass";
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;

		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

		windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

		windowClass.lpszClassName = className;
		windowClass.lpszMenuName = nullptr;

		windowClass.hInstance = HINSTANCE();
		windowClass.lpfnWndProc = HandleMsgSetup;

		int success = RegisterClassEx(&windowClass);
		SM_ASSERT(success, "Could not register window class!")

		// Create and display the window

		auto windowTitle = std::wstring{ settings.Title.begin(), settings.Title.end() };
		m_WindowHandle = CreateWindow(className,
				windowTitle.c_str(),
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				settings.Width,
				settings.Height,
				nullptr,
				nullptr,
				HINSTANCE(),
				this);

		SM_ASSERT(m_WindowHandle, "Could not create window!")
		
		ShowWindow(m_WindowHandle, SW_SHOW);
		SM_INFO("Window '%s' created", settings.Title.c_str());

		SetVSync(true);
		bInitialized = true;
	}

	void WindowsWindow::Update()
	{
		MSG msg{ 0 };
		while (msg.message != WM_QUIT)
		{
			// If there are window messages then process them
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void WindowsWindow::SetVSync(bool bEnabled)
	{
		m_Data.bVSync = bEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.bVSync;
	}

	LRESULT CALLBACK WindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
		}
		else
		{
			WindowsWindow* const pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (pWindow)
				return pWindow->HandleMsg(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if(!bInitialized)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		WindowsWindow* const pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (msg)
		{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			WindowCloseEvent event{};
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_SIZE:
		{
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);
			WindowResizeEvent event{ width, height };
			pWindow->m_Data.EventCallback(event);
			pWindow->m_Data.Width = width;
			pWindow->m_Data.Height = height;
		
			break;
		}

		case WM_KEYDOWN:
		{
			KeyPressedEvent event{ static_cast<unsigned char>(wParam), lParam & 0xFF };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_KEYUP:
		{
			KeyReleasedEvent event{ static_cast<unsigned char>(wParam) };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent event{ static_cast<unsigned char>(wParam) };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent event{ static_cast<unsigned char>(wParam) };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			MouseScrolledEvent event{ 0, zDelta };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		case WM_MOUSEMOVE:
		{
			float xPos = static_cast<float>(GET_X_LPARAM(lParam));
			float yPos = static_cast<float>(GET_Y_LPARAM(lParam));
			MouseMovedEvent event{ xPos, yPos };
			pWindow->m_Data.EventCallback(event);
			break;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
}
