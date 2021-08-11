#pragma once

#include "SmileEngine/Window.h"

#define HINSTANCE() GetModuleHandle(NULL) // this function returns the hInstance

namespace Smile
{
	class WindowsWindow final : public Window
	{
	public:
		WindowsWindow(const WindowSettings& settings);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool bEnabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowSettings& settings);
		virtual void ShutDown();

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		struct WindowData
		{
			std::string Title{};
			unsigned int Width{};
			unsigned int Height{};
			bool bVSync{};
			EventCallbackFunction EventCallback{};
		};

		WindowData m_Data;
		HWND m_WindowHandle;
		MSG m_Message;
		bool bInitialized = false;
	};
}

