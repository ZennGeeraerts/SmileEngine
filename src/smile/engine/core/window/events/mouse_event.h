/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "event.h"
#include "smile/engine/core/input/mouse_codes.h"

namespace smile::window
{
    class MouseMovedEvent final : public Event
    {
      public:
        MouseMovedEvent( float x, float y ) : m_MouseX{ x }, m_MouseY{ y }
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

        EVENT_CLASS_TYPE( MouseMoved )
        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      private:
        float m_MouseX;
        float m_MouseY;
    };

    class MouseScrolledEvent final : public Event
    {
      public:
        MouseScrolledEvent( float offsetX, float offsetY ) : m_OffsetX{ offsetX }, m_OffsetY{ offsetY }
        {
        }

        inline float GetOffsetX() const
        {
            return m_OffsetX;
        }
        inline float GetOffsetY() const
        {
            return m_OffsetY;
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "MouseScrolledEvent: " << GetOffsetX() << ", " << GetOffsetY();
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseScrolled )
        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      private:
        // To support horizontal scrolling on some mice
        float m_OffsetX;
        float m_OffsetY;
    };

    class MouseButtonEvent : public Event
    {
      public:
        virtual ~MouseButtonEvent() = default;

        inline int GetMouseButton() const
        {
            return m_Button;
        }

        EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput )

      protected:
        MouseButtonEvent( input::MouseCode button ) : m_Button{ button }
        {
        }

      protected:
        input::MouseCode m_Button;
    };

    class MouseButtonPressedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonPressedEvent( input::MouseCode button ) : MouseButtonEvent{ button }
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseButtonPressed )
    };

    class MouseButtonReleasedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonReleasedEvent( input::MouseCode button ) : MouseButtonEvent{ button }
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE( MouseButtonReleased )
    };
}
