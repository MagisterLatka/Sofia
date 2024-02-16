#pragma once

#include "Sofia/Events/Event.h"
#include "Sofia/Timestep.h"

namespace Sofia {

	class SOF_CORE WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int xsize, int ysize) noexcept
			: m_XSize(xsize), m_YSize(ysize) {}

		int GetXSize() const noexcept { return m_XSize; }
		int GetYSize() const noexcept { return m_YSize; }

		std::string ToString() const noexcept override
		{
			std::ostringstream oss;
			oss << "WindowResizeEvent: " << m_XSize << ", " << m_YSize;
			return oss.str();
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowResized)
	private:
		int m_XSize, m_YSize;
	};

	class SOF_CORE WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		std::string ToString() const noexcept override
		{
			return "WindowCloseEvent";
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowClosed)
	};

	class SOF_CORE WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() = default;

		std::string ToString() const noexcept override
		{
			return "WindowFocusEvent";
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowFocused)
	};

	class SOF_CORE WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() = default;

		std::string ToString() const noexcept override
		{
			return "WindowLostFocusEvent";
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowLostFocus)
	};

	class SOF_CORE WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xpos, int ypos) noexcept
			: m_XPos(xpos), m_YPos(ypos) {}

		int GetXPos() const noexcept { return m_XPos; }
		int GetYPos() const noexcept { return m_YPos; }

		std::string ToString() const noexcept override
		{
			std::ostringstream oss;
			oss << "WindowMoveEvent: " << m_XPos << ", " << m_YPos;
			return oss.str();
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowMoved)
	private:
		int m_XPos, m_YPos;
	};

	class SOF_CORE AppTickEvent : public Event
	{
	public:
		AppTickEvent(Timestep ts) noexcept
			: m_Timestep(ts) {}

		Timestep GetTimestep() const { return m_Timestep; }

		std::string ToString() const noexcept override
		{
			std::ostringstream oss;
			oss << "AppTickEvent: " << (double)m_Timestep;
			return oss.str();
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppTick)
	private:
		Timestep m_Timestep;
	};

	class SOF_CORE AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		std::string ToString() const noexcept override
		{
			return "AppUpdateEvent";
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppUpdate)
	};

	class SOF_CORE AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		std::string ToString() const noexcept override
		{
			return "AppRenderEvent";
		}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppRender)
	};
}
