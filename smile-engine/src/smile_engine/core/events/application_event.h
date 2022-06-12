#pragma once
#include "event.h"

namespace smile
{
    class WindowResizeEvent final : public Event
    {
      public:
        WindowResizeEvent( Uint32 width, Uint32 height ) : width{ width }, height{ height }
        {
        }

        inline Uint32 getWidth() const
        {
            return width;
        }
        inline Uint32 getHeight() const
        {
            return height;
        }

        std::string toString() const override
        {
            std::stringstream ss{};
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }

        EVENT_CLASS_TYPE( WindowResize )
        EVENT_CLASS_CATEGORY( EventCategoryApplication )

      private:
        Uint32 width;
        Uint32 height;
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