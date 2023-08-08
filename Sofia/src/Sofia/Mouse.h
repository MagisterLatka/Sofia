#pragma once

namespace Sofia {

	class Mouse
	{
		friend class WindowsWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid
			};
		public:
			Event() noexcept
				: m_Type(Type::Invalid), m_IsLeftPressed(false), m_IsRightPressed(false), m_Pos(0, 0) {}
			Event(Type type, const Mouse& mouse) noexcept
				: m_Type(type), m_IsLeftPressed(mouse.m_IsLeftPressed), m_IsRightPressed(mouse.m_IsRightPressed), m_Pos(mouse.m_Pos) {}

			bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			Type GetType() const noexcept { return m_Type; }

			glm::ivec2 GetPos() const noexcept { return m_Pos; }

			bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
			bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
		private:
			Type m_Type;
			bool m_IsLeftPressed, m_IsRightPressed;
			glm::ivec2 m_Pos;
		};
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;

		Mouse& operator=(const Mouse&) = delete;

		glm::ivec2 GetPos() const noexcept { return m_Pos; }
		bool IsInWindow() const noexcept { return m_IsInWindow; }
		bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
		bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
		Mouse::Event ReadEvent() noexcept;
		bool IsEmpty() const noexcept { return m_Buffer.empty(); }
		void Flush() noexcept;
	private:
		void OnMouseMove( int x,int y ) noexcept;
		void OnMouseLeave() noexcept;
		void OnMouseEnter() noexcept;
		void OnLeftPressed( int x,int y ) noexcept;
		void OnLeftReleased( int x,int y ) noexcept;
		void OnRightPressed( int x,int y ) noexcept;
		void OnRightReleased( int x,int y ) noexcept;
		void OnWheelUp( int x,int y ) noexcept;
		void OnWheelDown( int x,int y ) noexcept;
		void TrimBuffer() noexcept;
		void OnWheelDelta( int x,int y,int delta ) noexcept;
	private:
		static constexpr uint32_t c_BufferSize = 16u;
		glm::ivec2 m_Pos;
		bool m_IsLeftPressed = false, m_IsRightPressed = false;
		bool m_IsInWindow = false;
		int m_WheelDelta = 0;
		std::queue<Event> m_Buffer;
	};
}
