#pragma once

namespace Sofia
{
	enum class KeyCode : uint16_t
	{
		Space = 0x20u,
		Apostrophe = 0xC0u, /* ' */
		Comma = 0xBCu, /* u, */
		Minus = 0xBDu, /* - */
		Period = 0xBEu, /* . */
		Slash = 0xBFu, /* / */

		D0 = 0x30u, /* 0 */
		D1 = 0x31u, /* 1 */
		D2 = 0x32u, /* 2 */
		D3 = 0x33u, /* 3 */
		D4 = 0x34u, /* 4 */
		D5 = 0x35u, /* 5 */
		D6 = 0x36u, /* 6 */
		D7 = 0x37u, /* 7 */
		D8 = 0x38u, /* 8 */
		D9 = 0x39u, /* 9 */

		Semicolon = 0xBAu, /* ; */
		Equal = 0xBBu, /* = */

		A = 0x41u,
		B = 0x42u,
		C = 0x43u,
		D = 0x44u,
		E = 0x45u,
		F = 0x46u,
		G = 0x47u,
		H = 0x48u,
		I = 0x49u,
		J = 0x4Au,
		K = 0x4Bu,
		L = 0x4Cu,
		M = 0x4Du,
		N = 0x4Eu,
		O = 0x4Fu,
		P = 0x50u,
		Q = 0x51u,
		R = 0x52u,
		S = 0x53u,
		T = 0x54u,
		U = 0x55u,
		V = 0x56u,
		W = 0x57u,
		X = 0x58u,
		Y = 0x59u,
		Z = 0x5Au,

		LeftBracket = 0xDBu,  /* [ */
		Backslash = 0xDCu,  /* \ */
		RightBracket = 0xDDu,  /* ] */
		GraveAccent = 0xDEu,  /* ` */

		Escape = 0x1Bu,
		Enter = 0x0Du,
		Tab = 0x09u,
		Backspace = 0x08u,
		Insert = 0x2Du,
		Delete = 0x2Eu,
		Right = 0x27u,
		Left = 0x25u,
		Down = 0x28u,
		Up = 0x26u,
		PageUp = 0x21u,
		PageDown = 0x22u,
		Home = 0x24u,
		End = 0x23u,
		CapsLock = 0x14u,
		ScrollLock = 0x91u,
		NumLock = 0x90u,
		PrintScreen = 0x2Cu,
		Pause = 0x13u,
		F1 = 0x70u,
		F2 = 0x71u,
		F3 = 0x72u,
		F4 = 0x73u,
		F5 = 0x74u,
		F6 = 0x75u,
		F7 = 0x76u,
		F8 = 0x77u,
		F9 = 0x78u,
		F10 = 0x79u,
		F11 = 0x7Au,
		F12 = 0x7Bu,
		F13 = 0x7Cu,
		F14 = 0x7Du,
		F15 = 0x7Eu,
		F16 = 0x7Fu,
		F17 = 0x80u,
		F18 = 0x81u,
		F19 = 0x82u,
		F20 = 0x83u,
		F21 = 0x84u,
		F22 = 0x85u,
		F23 = 0x86u,
		F24 = 0x87u,

		/* Keypad */
		KP0 = 0x60u,
		KP1 = 0x61u,
		KP2 = 0x62u,
		KP3 = 0x63u,
		KP4 = 0x64u,
		KP5 = 0x65u,
		KP6 = 0x66u,
		KP7 = 0x67u,
		KP8 = 0x68u,
		KP9 = 0x69u,
		KPDecimal = 0x6Eu,
		KPDivide = 0x6Fu,
		KPMultiply = 0x6Au,
		KPSubtract = 0x6Du,
		KPAdd = 0x6Bu,

		LeftShift = 0xA0u,
		LeftControl = 0xA2u,
		LeftAlt = 0xA4u,
		RightShift = 0xA1u,
		RightControl = 0xA3u,
		RightAlt = 0xA5u,
		Menu = 0x5Bu
	};

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) noexcept
	{
		os << static_cast<uint32_t>(keyCode);
		return os;
	}
	inline uint8_t GetVKCode(KeyCode keyCode) noexcept
	{
		return (uint8_t)keyCode;
	}
}

#define SOF_KEY_SPACE			::Sofia::KeyCode::Space
#define SOF_KEY_APOSTROPHE		::Sofia::KeyCode::Apostrophe    /* ' */
#define SOF_KEY_COMMA			::Sofia::KeyCode::Comma         /* , */
#define SOF_KEY_MINUS			::Sofia::KeyCode::Minus         /* - */
#define SOF_KEY_PERIOD			::Sofia::KeyCode::Period        /* . */
#define SOF_KEY_SLASH			::Sofia::KeyCode::Slash         /* / */
#define SOF_KEY_0				::Sofia::KeyCode::D0
#define SOF_KEY_1				::Sofia::KeyCode::D1
#define SOF_KEY_2				::Sofia::KeyCode::D2
#define SOF_KEY_3				::Sofia::KeyCode::D3
#define SOF_KEY_4				::Sofia::KeyCode::D4
#define SOF_KEY_5				::Sofia::KeyCode::D5
#define SOF_KEY_6				::Sofia::KeyCode::D6
#define SOF_KEY_7				::Sofia::KeyCode::D7
#define SOF_KEY_8				::Sofia::KeyCode::D8
#define SOF_KEY_9				::Sofia::KeyCode::D9
#define SOF_KEY_SEMICOLON		::Sofia::KeyCode::Semicolon     /* ; */
#define SOF_KEY_EQUAL			::Sofia::KeyCode::Equal         /* = */
#define SOF_KEY_A				::Sofia::KeyCode::A
#define SOF_KEY_B				::Sofia::KeyCode::B
#define SOF_KEY_C				::Sofia::KeyCode::C
#define SOF_KEY_D				::Sofia::KeyCode::D
#define SOF_KEY_E				::Sofia::KeyCode::E
#define SOF_KEY_F				::Sofia::KeyCode::F
#define SOF_KEY_G				::Sofia::KeyCode::G
#define SOF_KEY_H				::Sofia::KeyCode::H
#define SOF_KEY_I				::Sofia::KeyCode::I
#define SOF_KEY_J				::Sofia::KeyCode::J
#define SOF_KEY_K				::Sofia::KeyCode::K
#define SOF_KEY_L				::Sofia::KeyCode::L
#define SOF_KEY_M				::Sofia::KeyCode::M
#define SOF_KEY_N				::Sofia::KeyCode::N
#define SOF_KEY_O				::Sofia::KeyCode::O
#define SOF_KEY_P				::Sofia::KeyCode::P
#define SOF_KEY_Q				::Sofia::KeyCode::Q
#define SOF_KEY_R				::Sofia::KeyCode::R
#define SOF_KEY_S				::Sofia::KeyCode::S
#define SOF_KEY_T				::Sofia::KeyCode::T
#define SOF_KEY_U				::Sofia::KeyCode::U
#define SOF_KEY_V				::Sofia::KeyCode::V
#define SOF_KEY_W				::Sofia::KeyCode::W
#define SOF_KEY_X				::Sofia::KeyCode::X
#define SOF_KEY_Y				::Sofia::KeyCode::Y
#define SOF_KEY_Z				::Sofia::KeyCode::Z
#define SOF_KEY_LEFT_BRACKET	::Sofia::KeyCode::LeftBracket   /* [ */
#define SOF_KEY_BACKSLASH		::Sofia::KeyCode::Backslash     /* \ */
#define SOF_KEY_RIGHT_BRACKET	::Sofia::KeyCode::RightBracket  /* ] */
#define SOF_KEY_GRAVE_ACCENT	::Sofia::KeyCode::GraveAccent   /* ` */

#define SOF_KEY_ESCAPE			::Sofia::KeyCode::Escape
#define SOF_KEY_ENTER			::Sofia::KeyCode::Enter
#define SOF_KEY_TAB				::Sofia::KeyCode::Tab
#define SOF_KEY_BACKSPACE		::Sofia::KeyCode::Backspace
#define SOF_KEY_INSERT			::Sofia::KeyCode::Insert
#define SOF_KEY_DELETE			::Sofia::KeyCode::Delete
#define SOF_KEY_RIGHT			::Sofia::KeyCode::Right
#define SOF_KEY_LEFT			::Sofia::KeyCode::Left
#define SOF_KEY_DOWN			::Sofia::KeyCode::Down
#define SOF_KEY_UP				::Sofia::KeyCode::Up
#define SOF_KEY_PAGE_UP			::Sofia::KeyCode::PageUp
#define SOF_KEY_PAGE_DOWN		::Sofia::KeyCode::PageDown
#define SOF_KEY_HOME			::Sofia::KeyCode::Home
#define SOF_KEY_END				::Sofia::KeyCode::End
#define SOF_KEY_CAPS_LOCK		::Sofia::KeyCode::CapsLock
#define SOF_KEY_SCROLL_LOCK		::Sofia::KeyCode::ScrollLock
#define SOF_KEY_NUM_LOCK		::Sofia::KeyCode::NumLock
#define SOF_KEY_PRINT_SCREEN	::Sofia::KeyCode::PrintScreen
#define SOF_KEY_PAUSE			::Sofia::KeyCode::Pause
#define SOF_KEY_F1				::Sofia::KeyCode::F1
#define SOF_KEY_F2				::Sofia::KeyCode::F2
#define SOF_KEY_F3				::Sofia::KeyCode::F3
#define SOF_KEY_F4				::Sofia::KeyCode::F4
#define SOF_KEY_F5				::Sofia::KeyCode::F5
#define SOF_KEY_F6				::Sofia::KeyCode::F6
#define SOF_KEY_F7				::Sofia::KeyCode::F7
#define SOF_KEY_F8				::Sofia::KeyCode::F8
#define SOF_KEY_F9				::Sofia::KeyCode::F9
#define SOF_KEY_F10				::Sofia::KeyCode::F10
#define SOF_KEY_F11				::Sofia::KeyCode::F11
#define SOF_KEY_F12				::Sofia::KeyCode::F12
#define SOF_KEY_F13				::Sofia::KeyCode::F13
#define SOF_KEY_F14				::Sofia::KeyCode::F14
#define SOF_KEY_F15				::Sofia::KeyCode::F15
#define SOF_KEY_F16				::Sofia::KeyCode::F16
#define SOF_KEY_F17				::Sofia::KeyCode::F17
#define SOF_KEY_F18				::Sofia::KeyCode::F18
#define SOF_KEY_F19				::Sofia::KeyCode::F19
#define SOF_KEY_F20				::Sofia::KeyCode::F20
#define SOF_KEY_F21				::Sofia::KeyCode::F21
#define SOF_KEY_F22				::Sofia::KeyCode::F22
#define SOF_KEY_F23				::Sofia::KeyCode::F23
#define SOF_KEY_F24				::Sofia::KeyCode::F24
#define SOF_KEY_F25				::Sofia::KeyCode::F25

#define SO_FKEY_KP_0			::Sofia::KeyCode::KP0
#define SO_FKEY_KP_1			::Sofia::KeyCode::KP1
#define SO_FKEY_KP_2			::Sofia::KeyCode::KP2
#define SOF_KEY_KP_3			::Sofia::KeyCode::KP3
#define SOF_KEY_KP_4			::Sofia::KeyCode::KP4
#define SOF_KEY_KP_5			::Sofia::KeyCode::KP5
#define SOF_KEY_KP_6			::Sofia::KeyCode::KP6
#define SOF_KEY_KP_7			::Sofia::KeyCode::KP7
#define SOF_KEY_KP_8			::Sofia::KeyCode::KP8
#define SOF_KEY_KP_9			::Sofia::KeyCode::KP9
#define SOF_KEY_KP_DECIMAL		::Sofia::KeyCode::KPDecimal
#define SOF_KEY_KP_DIVIDE		::Sofia::KeyCode::KPDivide
#define SOF_KEY_KP_MULTIPLY		::Sofia::KeyCode::KPMultiply
#define SOF_KEY_KP_SUBTRACT		::Sofia::KeyCode::KPSubtract
#define SOF_KEY_KP_ADD			::Sofia::KeyCode::KPAdd

#define SOF_KEY_LEFT_SHIFT		::Sofia::KeyCode::LeftShift
#define SOF_KEY_LEFT_CONTROL	::Sofia::KeyCode::LeftControl
#define SOF_KEY_LEFT_ALT		::Sofia::KeyCode::LeftAlt
#define SOF_KEY_RIGHT_SHIFT		::Sofia::KeyCode::RightShift
#define SOF_KEY_RIGHT_CONTROL	::Sofia::KeyCode::RightControl
#define SOF_KEY_RIGHT_ALT		::Sofia::KeyCode::RightAlt
#define SOF_KEY_MENU			::Sofia::KeyCode::Menu
