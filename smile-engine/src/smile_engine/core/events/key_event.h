#pragma once

#include "Event.h"
#include "smile_engine/input/key_codes.h"

namespace smile
{
    class KeyEvent : public Event
    {
      public:
        virtual ~KeyEvent() = default;

        inline input::KeyCode getKeyCode() const
        {
            return keyCode;
        }

        EVENT_CLASS_CATEGORY( EventCategoryKeyboard | EventCategoryInput )

      protected:
        KeyEvent( input::KeyCode key_code ) : keyCode{ key_code }
        {
        }

        input::KeyCode keyCode;
    };

    class KeyPressedEvent final : public KeyEvent
    {
      public:
        KeyPressedEvent( input::KeyCode key_code, Uint16 repeat_count )
            : KeyEvent( key_code ), repeatCount{ repeat_count }
        {
        }

        inline Uint16 getRepeatCount() const
        {
            return repeatCount;
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyPressed );

      private:
        Uint16 repeatCount;
    };

    class KeyReleasedEvent final : public KeyEvent
    {
      public:
        KeyReleasedEvent( input::KeyCode key_code ) : KeyEvent( key_code )
        {
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "KeyReleasedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyReleased );
    };

    class KeyTypedEvent final : public KeyEvent
    {
      public:
        KeyTypedEvent( input::KeyCode key_code ) : KeyEvent( key_code )
        {
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "KeyTypedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE( KeyTyped );
    };
}