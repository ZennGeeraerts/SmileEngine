/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "event.h"
#include "engine/core/input/key_codes.h"

namespace smile::window
{
    class KeyEvent : public Event
    {
      public:
        virtual ~KeyEvent() = default;

        inline input::KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

        EVENT_CLASS_CATEGORY( EventCategoryKeyboard | EventCategoryInput )

      protected:
        KeyEvent( input::KeyCode key_code ) : m_KeyCode{ key_code }
        {
        }

        input::KeyCode m_KeyCode;
    };

    class KeyPressedEvent final : public KeyEvent
    {
      public:
        KeyPressedEvent( input::KeyCode keyCode, Uint16 repeatCount )
            : KeyEvent{ keyCode }, m_RepeatCount{ repeatCount }
        {
        }

        inline Uint16 GetRepeatCount() const
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
        KeyReleasedEvent( input::KeyCode keyCode ) : KeyEvent{ keyCode }
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
        KeyTypedEvent( input::KeyCode keyCode ) : KeyEvent{ keyCode }
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