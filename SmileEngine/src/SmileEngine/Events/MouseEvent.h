#pragma once

#include "Event.h"

namespace Smile
{
	class SMILE_API MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX{ x }
			, m_MouseY{ y }
		{
		}

		inline float GetX() const
		{
			return m_MouseX;
		}

		inline float GetY() const
		{
			return m_MouseY;
		}

		std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(eMouseMoved)
		EVENT_CLASS_CATEGORY(eEventCategoryMouse | eEventCategoryInput)
	private:
		float m_MouseX;
		float m_MouseY;
	};

	class SMILE_API MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_OffsetX{ offsetX }
			, m_OffsetY{ offsetY }
		{
		}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "MouseScrolledEvent: " << GetOffsetX() << ", " << GetOffsetY();
			return ss.str();
		}

		EVENT_CLASS_TYPE(eMouseScrolled)
		EVENT_CLASS_CATEGORY(eEventCategoryMouse | eEventCategoryInput)
	private:
		float m_OffsetX;
		// To support horizontal scrolling on some mice
		float m_OffsetY;
	};
}
