#pragma once

#include "Event.h"

namespace Smile
{
	class SMILE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const
		{
			return m_KeyCode;
		}

		EVENT_CLASS_CATEGORY(eEventCategoryKeyboard | eEventCategoryInput)
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode{ keyCode }
		{
		}

		int m_KeyCode;
	};

	class SMILE_API KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode)
			, m_RepeatCount{ repeatCount }
		{
		}

		inline int GetRepeatCount() const
		{
			return m_RepeatCount;
		}

		std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(eKeyPressed);
	private:
		int m_RepeatCount;
	};

	class SMILE_API KeyReleasedEvent final : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(eKeyReleased);
	};
}