#include <pch.h>
#include "Mouse.h"

namespace Sofia {

	Mouse::Event Mouse::ReadEvent() noexcept
	{
		if (!m_Buffer.empty())
		{
			Event e = m_Buffer.front();
			m_Buffer.pop();
			return e;
		}
		return Event();
	}
	void Mouse::Flush() noexcept
	{
		m_Buffer = std::queue<Event>();
	}

	void Mouse::OnMouseMove(int x, int y) noexcept
	{
		m_Pos = { x, y };
		m_Buffer.push(Event(Event::Type::Move, *this));
		TrimBuffer();
	}
	void Mouse::OnMouseLeave() noexcept
	{
		m_IsInWindow = false;
		m_Buffer.push(Event(Event::Type::Leave, *this));
		TrimBuffer();
	}
	void Mouse::OnMouseEnter() noexcept
	{
		m_IsInWindow = true;
		m_Buffer.push(Event(Event::Type::Enter, *this));
		TrimBuffer();
	}
	void Mouse::OnLeftPressed(int x, int y) noexcept
	{
		m_IsLeftPressed = true;
		m_Buffer.push(Event(Event::Type::LPress, *this));
		TrimBuffer();
	}
	void Mouse::OnLeftReleased(int x, int y) noexcept
	{
		m_IsLeftPressed = false;
		m_Buffer.push(Event(Event::Type::LRelease, *this));
		TrimBuffer();
	}
	void Mouse::OnRightPressed(int x, int y) noexcept
	{
		m_IsRightPressed = true;
		m_Buffer.push(Event(Event::Type::RPress, *this));
		TrimBuffer();
	}
	void Mouse::OnRightReleased(int x, int y) noexcept
	{
		m_IsRightPressed = false;
		m_Buffer.push(Event(Event::Type::RRelease, *this));
		TrimBuffer();
	}
	void Mouse::OnWheelUp(int x, int y) noexcept
	{
		m_Buffer.push(Event(Event::Type::WheelUp, *this));
		TrimBuffer();
	}
	void Mouse::OnWheelDown(int x, int y) noexcept
	{
		m_Buffer.push(Event(Event::Type::WheelDown, *this));
		TrimBuffer();
	}
#if !defined(WHEEL_DELTA)
#	define WHEEL_DELTA 120
#endif
	void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
	{
		m_WheelDelta += delta;
		while (m_WheelDelta >= WHEEL_DELTA)
		{
			m_WheelDelta -= WHEEL_DELTA;
			OnWheelUp(x, y);
		}
		while (m_WheelDelta <= -WHEEL_DELTA)
		{
			m_WheelDelta += WHEEL_DELTA;
			OnWheelDown(x, y);
		}
	}

	void Mouse::TrimBuffer() noexcept
	{
		while (m_Buffer.size() > c_BufferSize)
			m_Buffer.pop();
	}
}
