#pragma once
#include "Event.h"

namespace Smile
{
	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width{ width }
			, m_Height{ height }
		{
		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(eWindowResize)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(eWindowClose)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	};
}