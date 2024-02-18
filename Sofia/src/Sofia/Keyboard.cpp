#include <pch.h>
#include "Keyboard.h"

namespace Sofia {

	Keyboard::Event Keyboard::ReadKey() noexcept
	{
		if (!m_KeyBuffer.empty())
		{
			Event e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}
		return Event();
	}
	void Keyboard::FlushKey() noexcept
	{
		m_KeyBuffer = std::queue<Event>();
	}

	char Keyboard::ReadChar() noexcept
	{
		if (!m_CharBuffer.empty())
		{
			uint8_t charcode = m_CharBuffer.front();
			m_CharBuffer.pop();
			return charcode;
		}
		return 0;
	}
	void Keyboard::FlushChar() noexcept
	{
		m_CharBuffer = std::queue<char>();
	}

	void Keyboard::Flush() noexcept
	{
		FlushKey();
		FlushChar();
	}

	void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
	{
		m_KeyStates[keycode] = true;
		m_KeyBuffer.push(Event(Event::Type::Pressed, keycode));
		TrimBuffer(m_KeyBuffer);
	}
	void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
	{
		m_KeyStates[keycode] = false;
		m_KeyBuffer.push(Event(Event::Type::Released, keycode));
		TrimBuffer(m_KeyBuffer);
	}
	void Keyboard::OnChar(char character) noexcept
	{
		m_CharBuffer.push(character);
		TrimBuffer(m_CharBuffer);
	}
	void Keyboard::ClearState() noexcept
	{
		m_KeyStates.reset();
	}
	template<typename T>
	void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > c_BufferSize)
			buffer.pop();
	}
}
