#pragma once
#include "smpch.h"
#include "SmileEngine/Core.h"

namespace Smile
{
	enum class EventType
	{
		eNone = 0,
		eWindowClose, eWindowResize, eWindowFocus, eWindowLostFocus, eWindowMoved,
		eKeyPressed, eKeyReleased, eKeyTyped,
		eMouseButtonPressed, eMouseButtonReleased, eMouseMoved, eMouseScrolled
	};

	enum EventCategory
	{
		eNone = 0,
		eEventCategoryApplication		= BIT(0),
		eEventCategoryInput				= BIT(1),
		eEventCategoryKeyboard			= BIT(2),
		eEventCategoryMouse				= BIT(3),
		eEventCategoryMouseButton		= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;
		friend class SmileGame;
	public:
		// GetName and ToString is not optimal and should only be used for debugging, not in the actual game
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const
		{
			return GetName();
		}

#pragma warning(push)
#pragma warning(disable: 26812)
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
#pragma warning(pop)

	protected:
		bool m_bHandled = false;
	};

	class EventDispatcher
	{
		template<typename ParameterType>
		using EventFunction = std::function<bool(ParameterType&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// Bind events to functions
		template<typename ParameterType>
		bool Dispatch(EventFunction<ParameterType> eventFunction)
		{
			if (m_Event.GetEventType() == ParameterType::GetStaticType())
			{
				m_Event.m_bHandled = eventFunction(*(ParameterType*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

