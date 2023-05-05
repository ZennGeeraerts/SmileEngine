/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
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
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::##type;                                                                                      \
    }                                                                                                                  \
    virtual EventType GetEventType() const override                                                                    \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char *GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

#define EVENT_CLASS_CATEGORY( category )                                                                               \
    virtual int GetCategoryFlags() const override                                                                      \
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
        virtual EventType GetEventType() const = 0;
        virtual const char *GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const
        {
            return GetName();
        }

#pragma warning( push )
#pragma warning( disable : 26812 )
        inline bool IsInCategory( EventCategory category )
        {
            return GetCategoryFlags() & category;
        }
#pragma warning( pop )

      protected:
        bool m_IsHandled = false;
    };

    class EventDispatcher
    {
        template < typename ParameterType >
        using EventFunction = std::function< bool( ParameterType & ) >;

      public:
        EventDispatcher( Event &event ) : m_Event( event )
        {
        }

        // Bind events to functions
        template < typename ParameterType >
        bool Dispatch( EventFunction< ParameterType > eventFunction )
        {
            if ( m_Event.GetEventType() == ParameterType::GetStaticType() )
            {
                m_Event.m_IsHandled = eventFunction( *( ParameterType * )&m_Event );
                return true;
            }
            return false;
        }

      private:
        Event &m_Event;
    };

    inline std::ostream &operator<<( std::ostream &os, const Event &e )
    {
        return os << e.ToString();
    }
}
