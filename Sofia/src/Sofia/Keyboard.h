#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Keyboard
	{
		friend class WindowsWindow;
		friend class LinuxWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				Pressed, Released, Invalid
			};
		public:
			SOF_CORE Event() noexcept
				: m_Type(Type::Invalid), m_Code(0u) {}
			SOF_CORE Event(Type type, uint8_t code) noexcept
				: m_Type(type), m_Code(code) {}

			SOF_CORE bool IsPressed() const noexcept { return m_Type == Type::Pressed; }
			SOF_CORE bool IsReleased() const noexcept { return m_Type == Type::Released; }
			SOF_CORE bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			SOF_CORE uint8_t GetCode() const noexcept { return m_Code; }
		private:
			Type m_Type;
			uint8_t m_Code;
		};
	public:
		SOF_CORE Keyboard() = default;
		Keyboard(const Keyboard&) = delete;

		SOF_CORE bool IsKeyPressed(uint8_t keycode) const noexcept { return m_KeyStates[keycode]; }
		SOF_CORE Event ReadKey() noexcept;
		SOF_CORE bool IsKeyBufferEmpty() const noexcept { return m_KeyBuffer.empty(); }
		SOF_CORE void FlushKey() noexcept;

		SOF_CORE char ReadChar() noexcept;
		SOF_CORE bool IsCharBufferEmpty() const noexcept { return m_CharBuffer.empty(); }
		SOF_CORE void FlushChar() noexcept;

		SOF_CORE void Flush() noexcept;

		SOF_CORE void EnableAutorepeat() noexcept { m_Autorepeat = true; }
		SOF_CORE void DisableAutorepeat() noexcept { m_Autorepeat = false; }
		SOF_CORE bool IsAutorepeatEnabled() const noexcept { return m_Autorepeat; }
	private:
		SOF_CORE void OnKeyPressed(unsigned char keycode) noexcept;
		SOF_CORE void OnKeyReleased(unsigned char keycode) noexcept;
		SOF_CORE void OnChar(char character) noexcept;
		SOF_CORE void ClearState() noexcept;
		template<typename T>
		SOF_CORE static void TrimBuffer(std::queue<T>& buffer) noexcept;
	private:
		static constexpr uint32_t c_NKeys = 256u;
		static constexpr uint32_t c_BufferSize = 16u;
		bool m_Autorepeat = false;
		std::bitset<c_NKeys> m_KeyStates;
		std::queue<Event> m_KeyBuffer;
		std::queue<char> m_CharBuffer;
	};
}
