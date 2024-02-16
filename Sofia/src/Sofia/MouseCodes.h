#pragma once

namespace Sofia
{
	enum class MouseCode : uint16_t
	{
		Button0 = 0u,
		Button1 = 1u,
		Button2 = 2u,
		Button3 = 3u,
		Button4 = 4u,
		Button5 = 5u,
		Button6 = 6u,
		Button7 = 7u,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) noexcept
	{
		os << static_cast<uint32_t>(mouseCode);
		return os;
	}
}

#define SOF_MOUSE_BUTTON_0		::Sofia::MouseCode::Button0
#define SOF_MOUSE_BUTTON_1		::Sofia::MouseCode::Button1
#define SOF_MOUSE_BUTTON_2		::Sofia::MouseCode::Button2
#define SOF_MOUSE_BUTTON_3		::Sofia::MouseCode::Button3
#define SOF_MOUSE_BUTTON_4		::Sofia::MouseCode::Button4
#define SOF_MOUSE_BUTTON_5		::Sofia::MouseCode::Button5
#define SOF_MOUSE_BUTTON_6		::Sofia::MouseCode::Button6
#define SOF_MOUSE_BUTTON_7		::Sofia::MouseCode::Button7
#define SOF_MOUSE_BUTTON_LAST	::Sofia::MouseCode::ButtonLast
#define SOF_MOUSE_BUTTON_LEFT	::Sofia::MouseCode::ButtonLeft
#define SOF_MOUSE_BUTTON_RIGHT	::Sofia::MouseCode::ButtonRight
#define SOF_MOUSE_BUTTON_MIDDLE	::Sofia::MouseCode::ButtonMiddle
