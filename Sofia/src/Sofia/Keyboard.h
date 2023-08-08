#pragma once

namespace Sofia {

	class Keyboard
	{
		friend class WindowsWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				Pressed, Released, Invalid
			};
		public:
			Event() noexcept
				: m_Type(Type::Invalid), m_Code(0u) {}
			Event(Type type, uint8_t code) noexcept
				: m_Type(type), m_Code(code) {}

			bool IsPressed() const noexcept { return m_Type == Type::Pressed; }
			bool IsReleased() const noexcept { return m_Type == Type::Released; }
			bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			uint8_t GetCode() const noexcept { return m_Code; }
		private:
			Type m_Type;
			uint8_t m_Code;
		};
	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;

		bool IsKeyPressed(uint8_t keycode) const noexcept { return m_KeyStates[keycode]; }
		Event ReadKey() noexcept;
		bool IsKeyBufferEmpty() const noexcept { return m_KeyBuffer.empty(); }
		void FlushKey() noexcept;

		char ReadChar() noexcept;
		bool IsCharBufferEmpty() const noexcept { return m_CharBuffer.empty(); }
		void FlushChar() noexcept;

		void Flush() noexcept;

		void EnableAutorepeat() noexcept { m_Autorepeat = true; }
		void DisableAutorepeat() noexcept { m_Autorepeat = false; }
		bool IsAutorepeatEnabled() const noexcept { return m_Autorepeat; }
	private:
		void OnKeyPressed(unsigned char keycode) noexcept;
		void OnKeyReleased(unsigned char keycode) noexcept;
		void OnChar(char character) noexcept;
		void ClearState() noexcept;
		template<typename T>
		static void TrimBuffer( std::queue<T>& buffer ) noexcept;
	private:
		static constexpr uint32_t c_NKeys = 256u;
		static constexpr uint32_t c_BufferSize = 16u;
		bool m_Autorepeat = false;
		std::bitset<c_NKeys> m_KeyStates;
		std::queue<Event> m_KeyBuffer;
		std::queue<char> m_CharBuffer;
	};
}
