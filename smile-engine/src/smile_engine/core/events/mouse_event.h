#pragma once

#include "Event.h"
#include "smile_engine/input/mouse_codes.h"

namespace smile
{
    class MouseMovedEvent final : public Event
    {
      public:
        MouseMovedEvent( float x, float y ) : mouseX{ x }, mouseY{ y }
        {
        }

        inline float getX() const
        {
            return mouseX;
        }

        inline float getY() const
        {
            return mouseY;
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseMoved )
        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      private:
        float mouseX;
        float mouseY;
    };

    class MouseScrolledEvent final : public Event
    {
      public:
        MouseScrolledEvent( float offset_x, float offset_y ) : offsetX{ offset_x }, offsetY{ offset_y }
        {
        }

        inline float getOffsetX() const
        {
            return offsetX;
        }
        inline float getOffsetY() const
        {
            return offsetY;
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "MouseScrolledEvent: " << getOffsetX() << ", " << getOffsetY();
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseScrolled )
        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      private:
        // To support horizontal scrolling on some mice
        float offsetX;
        float offsetY;
    };

    class MouseButtonEvent : public Event
    {
      public:
        virtual ~MouseButtonEvent() = default;

        inline int getMouseButton() const
        {
            return button;
        }

        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      protected:
        MouseButtonEvent( input::MouseCode button ) : button{ button }
        {
        }

      protected:
        input::MouseCode button;
    };

    class MouseButtonPressedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonPressedEvent( input::MouseCode button ) : MouseButtonEvent( button )
        {
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "MouseButtonPressedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseButtonPressed )
    };

    class MouseButtonReleasedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonReleasedEvent( input::MouseCode button ) : MouseButtonEvent( button )
        {
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "MouseButtonReleasedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseButtonReleased )
    };
}
