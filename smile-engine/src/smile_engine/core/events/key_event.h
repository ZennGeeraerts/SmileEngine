#pragma once

#include "Event.h"
#include "smile_engine/input/key_codes.h"

namespace Smile
{
    class KeyEvent : public Event
    {
      public:
        virtual ~KeyEvent() = default;

        inline Input::KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

        EVENT_CLASS_CATEGORY( EventCategoryKeyboard | EventCategoryInput )

      protected:
        KeyEvent( Input::KeyCode key_code ) : m_KeyCode{ key_code }
        {
        }

        Input::KeyCode m_KeyCode;
    };

    class KeyPressedEvent final : public KeyEvent
    {
      public:
        KeyPressedEvent( Input::KeyCode keyCode, Uint16 repeatCount )
            : KeyEvent{ keyCode }, m_RepeatCount{ repeatCount }
        {
        }

        inline Uint16 getRepeatCount() const
        {
            return m_RepeatCount;
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyPressed );

      private:
        Uint16 m_RepeatCount;
    };

    class KeyReleasedEvent final : public KeyEvent
    {
      public:
        KeyReleasedEvent( Input::KeyCode keyCode ) : KeyEvent{ keyCode }
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyReleased );
    };

    class KeyTypedEvent final : public KeyEvent
    {
      public:
        KeyTypedEvent( Input::KeyCode keyCode ) : KeyEvent{ keyCode }
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyTyped );
    };
}