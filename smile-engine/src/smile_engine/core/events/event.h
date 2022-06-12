#pragma once
#include "smpch.h"
#include "smile_engine/core/core.h"

namespace smile
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT( 0 ),
        EventCategoryInput = BIT( 1 ),
        EventCategoryKeyboard = BIT( 2 ),
        EventCategoryMouse = BIT( 3 ),
        EventCategoryMouseButton = BIT( 4 )
    };

#define EVENT_CLASS_TYPE( type )                                                                                       \
    static EventType getStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::##type;                                                                                      \
    }                                                                                                                  \
    virtual EventType getEventType() const override                                                                    \
    {                                                                                                                  \
        return getStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char *getName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

#define EVENT_CLASS_CATEGORY( category )                                                                               \
    virtual int getCategoryFlags() const override                                                                      \
    {                                                                                                                  \
        return category;                                                                                               \
    }

    class Event
    {
        friend class EventDispatcher;
        friend class Application;

      public:
        virtual ~Event() = default;

        // GetName and ToString is not optimal and should only be used for debugging, not in the actual game
        virtual EventType getEventType() const = 0;
        virtual const char *getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const
        {
            return getName();
        }

#pragma warning( push )
#pragma warning( disable : 26812 )
        inline bool isInCategory( EventCategory category )
        {
            return getCategoryFlags() & category;
        }
#pragma warning( pop )

      protected:
        bool isHandled = false;
    };

    class EventDispatcher
    {
        template < typename ParameterType >
        using EventFunction = std::function< bool( ParameterType & ) >;

      public:
        EventDispatcher( Event &event ) : event( event )
        {
        }

        // Bind events to functions
        template < typename ParameterType >
        bool dispatch( EventFunction< ParameterType > event_function )
        {
            if ( event.getEventType() == ParameterType::getStaticType() )
            {
                event.isHandled = event_function( *( ParameterType * )&event );
                return true;
            }
            return false;
        }

      private:
        Event &event;
    };

    inline std::ostream &operator<<( std::ostream &os, const Event &e )
    {
        return os << e.toString();
    }
}
