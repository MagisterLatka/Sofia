#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Mouse
	{
		friend class WindowsWindow;
		friend class LinuxWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid
			};
		public:
			SOF_CORE Event() noexcept
				: m_Type(Type::Invalid), m_IsLeftPressed(false), m_IsRightPressed(false), m_Pos(0, 0) {}
			SOF_CORE Event(Type type, const Mouse& mouse) noexcept
				: m_Type(type), m_IsLeftPressed(mouse.m_IsLeftPressed), m_IsRightPressed(mouse.m_IsRightPressed), m_Pos(mouse.m_Pos) {}

			SOF_CORE bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			SOF_CORE Type GetType() const noexcept { return m_Type; }

			SOF_CORE glm::ivec2 GetPos() const noexcept { return m_Pos; }

			SOF_CORE bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
			SOF_CORE bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
		private:
			Type m_Type;
			bool m_IsLeftPressed, m_IsRightPressed;
			glm::ivec2 m_Pos;
		};
	public:
		SOF_CORE Mouse() = default;
		Mouse(const Mouse&) = delete;

		Mouse& operator=(const Mouse&) = delete;

		SOF_CORE glm::ivec2 GetPos() const noexcept { return m_Pos; }
		SOF_CORE bool IsInWindow() const noexcept { return m_IsInWindow; }
		SOF_CORE bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
		SOF_CORE bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
		SOF_CORE Mouse::Event ReadEvent() noexcept;
		SOF_CORE bool IsEmpty() const noexcept { return m_Buffer.empty(); }
		SOF_CORE void Flush() noexcept;
	private:
		SOF_CORE void OnMouseMove(int x, int y) noexcept;
		SOF_CORE void OnMouseLeave() noexcept;
		SOF_CORE void OnMouseEnter() noexcept;
		SOF_CORE void OnLeftPressed(int x, int y) noexcept;
		SOF_CORE void OnLeftReleased(int x, int y) noexcept;
		SOF_CORE void OnRightPressed(int x, int y) noexcept;
		SOF_CORE void OnRightReleased(int x, int y) noexcept;
		SOF_CORE void OnWheelUp(int x, int y) noexcept;
		SOF_CORE void OnWheelDown(int x, int y) noexcept;
		SOF_CORE void TrimBuffer() noexcept;
		SOF_CORE void OnWheelDelta(int x, int y, int delta) noexcept;
	private:
		static constexpr uint32_t c_BufferSize = 16u;
		glm::ivec2 m_Pos;
		bool m_IsLeftPressed = false, m_IsRightPressed = false;
		bool m_IsInWindow = false;
		int m_WheelDelta = 0;
		std::queue<Event> m_Buffer;
	};
}
