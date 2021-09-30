#pragma once
#include "smpch.h"
#include "SmileEngine/Core/Core.h"
#include "SmileEngine/Events/Event.h"

namespace Smile
{
	struct WindowSettings
	{
		WindowSettings(const std::string& title = "Smile Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title{ title }
			, Width{ width }
			, Height{ height }
		{
		}

		std::string Title;
		uint32_t Width;
		uint32_t Height;
	};

	class RenderingContext;

	// Window interface for desktop platforms
	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual RenderingContext* GetRenderingContext() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool bEnabled) = 0;
		virtual bool IsVSync() const = 0;

		// Returns the child window 
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSettings& settings = WindowSettings{});
	};
}

