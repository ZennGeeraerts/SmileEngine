#pragma once

#include "SmileEngine/Window.h"
#include "SmileEngine/Renderer/RenderingContext.h"

class RenderingContext;

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
		inline RenderingContext* GetRenderingContext() const { return m_pContext; }
		inline HWND GetWindowHandle() const { return m_WindowHandle; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool bEnabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowSettings& settings);
		virtual void ShutDown();

		static LRESULT CALLBACK WindowsProcedureStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		
		void PollEvents();

	private:
		HWND m_WindowHandle;
		MSG m_Message;
		RenderingContext* m_pContext;

		struct WindowData
		{
			std::string Title{};
			unsigned int Width{};
			unsigned int Height{};
			bool bVSync{};
			EventCallbackFunction EventCallback{};
		};

		WindowData m_Data;
		bool m_bInitialized = false;
	};
}

