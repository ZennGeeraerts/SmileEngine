/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "event.h"

namespace smile
{
    class WindowResizeEvent final : public Event
    {
      public:
        WindowResizeEvent( Uint32 width, Uint32 height ) : m_Width{ width }, m_Height{ height }
        {
        }

        inline Uint32 GetWidth
        () const
        {
            return m_Width;
        }
        inline Uint32 GetHeight() const
        {
            return m_Height;
        }

        std::string ToString() const override
        {
            std::stringstream ss{};
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE( WindowResize )
        EVENT_CLASS_CATEGORY( EventCategoryApplication )

      private:
        Uint32 m_Width;
        Uint32 m_Height;
    };

    class WindowCloseEvent final : public Event
    {
      public:
        WindowCloseEvent()
        {
        }

        EVENT_CLASS_TYPE( WindowClose )
        EVENT_CLASS_CATEGORY( EventCategoryApplication )
    };
}