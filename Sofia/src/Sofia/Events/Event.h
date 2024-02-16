#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	enum class EventType
	{
		None = 0,
		WindowResized, WindowClosed, WindowFocused, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased, MouseEntered, MouseLeft
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_TYPE(x) static EventType GetStaticType() noexcept { return EventType::x; }\
						virtual EventType GetEventType() const noexcept override { return GetStaticType(); }\
						virtual const char* GetName() const noexcept override { return #x; }
#define EVENT_CATEGORY(x) virtual int GetCategoryFlags() const noexcept override { return x; }

	class SOF_CORE Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const noexcept = 0;
		virtual const char* GetName() const noexcept = 0;
		virtual int GetCategoryFlags() const noexcept = 0;
		virtual std::string ToString() const noexcept { return GetName(); }

		bool IsInCategory(EventCategory category) noexcept
		{
			return GetCategoryFlags() & category;
		}
	public:
		bool p_Handled = false;
	};

	class SOF_CORE Dispatcher
	{
	public:
		Dispatcher(Event& e) noexcept
			: m_Event(e)
		{}

		template<typename T, typename F>
		bool Dispatch(const F& fn) noexcept
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.p_Handled |= fn(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& e) noexcept
	{
		return stream << e.ToString();
	}
}
