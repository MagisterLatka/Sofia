#pragma once

namespace Sofia
{
	enum class KeyCode : uint8_t
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
	inline uint16_t GetGLFWCode(KeyCode keycode) noexcept
	{
		switch (keycode)
		{
		case KeyCode::Space:		return 0x20u;
		case KeyCode::Apostrophe:	return 0x27u;
		case KeyCode::Comma:		return 0x2Cu;
		case KeyCode::Minus:		return 0x2Du;
		case KeyCode::Period:		return 0x2Eu;
		case KeyCode::Slash:		return 0x2Fu;

		case KeyCode::D0: 			return 0x30u;
		case KeyCode::D1: 			return 0x31u;
		case KeyCode::D2: 			return 0x32u;
		case KeyCode::D3: 			return 0x33u;
		case KeyCode::D4: 			return 0x34u;
		case KeyCode::D5: 			return 0x35u;
		case KeyCode::D6: 			return 0x36u;
		case KeyCode::D7: 			return 0x37u;
		case KeyCode::D8: 			return 0x38u;
		case KeyCode::D9: 			return 0x39u;

		case KeyCode::Semicolon:	return 0x3Bu;
		case KeyCode::Equal:		return 0x3Du;

		case KeyCode::A:			return 0x41u;
		case KeyCode::B:			return 0x42u;
		case KeyCode::C:			return 0x43u;
		case KeyCode::D:			return 0x44u;
		case KeyCode::E:			return 0x45u;
		case KeyCode::F:			return 0x46u;
		case KeyCode::G:			return 0x47u;
		case KeyCode::H:			return 0x48u;
		case KeyCode::I:			return 0x49u;
		case KeyCode::J:			return 0x4Au;
		case KeyCode::K:			return 0x4Bu;
		case KeyCode::L:			return 0x4Cu;
		case KeyCode::M:			return 0x4Du;
		case KeyCode::N:			return 0x4Eu;
		case KeyCode::O:			return 0x4Fu;
		case KeyCode::P:			return 0x50u;
		case KeyCode::Q:			return 0x51u;
		case KeyCode::R:			return 0x52u;
		case KeyCode::S:			return 0x53u;
		case KeyCode::T:			return 0x54u;
		case KeyCode::U:			return 0x55u;
		case KeyCode::V:			return 0x56u;
		case KeyCode::W:			return 0x57u;
		case KeyCode::X:			return 0x58u;
		case KeyCode::Y:			return 0x59u;
		case KeyCode::Z:			return 0x5Au;

		case KeyCode::LeftBracket:	return 0x5Bu;
		case KeyCode::Backslash:	return 0x5Cu;
		case KeyCode::RightBracket:	return 0x5Du;
		case KeyCode::GraveAccent:	return 0x60u;

		case KeyCode::Escape:		return 0x100u;
		case KeyCode::Enter:		return 0x101u;
		case KeyCode::Tab:			return 0x102u;
		case KeyCode::Backspace:	return 0x103u;
		case KeyCode::Insert:		return 0x104u;
		case KeyCode::Delete:		return 0x105u;
		case KeyCode::Right:		return 0x106u;
		case KeyCode::Left:			return 0x107u;
		case KeyCode::Down:			return 0x108u;
		case KeyCode::Up:			return 0x109u;
		case KeyCode::PageUp:		return 0x10Au;
		case KeyCode::PageDown:		return 0x10Bu;
		case KeyCode::Home:			return 0x10Cu;
		case KeyCode::End:			return 0x10Du;
		case KeyCode::CapsLock:		return 0x118u;
		case KeyCode::ScrollLock:	return 0x119u;
		case KeyCode::NumLock:		return 0x11Au;
		case KeyCode::PrintScreen:	return 0x11Bu;
		case KeyCode::Pause:		return 0x11Cu;
		case KeyCode::F1:			return 0x122u;
		case KeyCode::F2:			return 0x123u;
		case KeyCode::F3:			return 0x124u;
		case KeyCode::F4:			return 0x125u;
		case KeyCode::F5:			return 0x126u;
		case KeyCode::F6:			return 0x127u;
		case KeyCode::F7:			return 0x128u;
		case KeyCode::F8:			return 0x129u;
		case KeyCode::F9:			return 0x12Au;
		case KeyCode::F10:			return 0x12Bu;
		case KeyCode::F11:			return 0x12Cu;
		case KeyCode::F12:			return 0x12Du;
		case KeyCode::F13:			return 0x12Eu;
		case KeyCode::F14:			return 0x12Fu;
		case KeyCode::F15:			return 0x130u;
		case KeyCode::F16:			return 0x131u;
		case KeyCode::F17:			return 0x132u;
		case KeyCode::F18:			return 0x133u;
		case KeyCode::F19:			return 0x134u;
		case KeyCode::F20:			return 0x135u;
		case KeyCode::F21:			return 0x136u;
		case KeyCode::F22:			return 0x137u;
		case KeyCode::F23:			return 0x138u;
		case KeyCode::F24:			return 0x139u;

		/* Keypad */
		case KeyCode::KP0:			return 0x140u;
		case KeyCode::KP1:			return 0x141u;
		case KeyCode::KP2:			return 0x142u;
		case KeyCode::KP3:			return 0x143u;
		case KeyCode::KP4:			return 0x144u;
		case KeyCode::KP5:			return 0x145u;
		case KeyCode::KP6:			return 0x146u;
		case KeyCode::KP7:			return 0x147u;
		case KeyCode::KP8:			return 0x148u;
		case KeyCode::KP9:			return 0x149u;
		case KeyCode::KPDecimal:	return 0x14Au;
		case KeyCode::KPDivide:		return 0x14Bu;
		case KeyCode::KPMultiply:	return 0x14Cu;
		case KeyCode::KPSubtract:	return 0x14Du;
		case KeyCode::KPAdd:		return 0x14Eu;

		case KeyCode::LeftShift:	return 0x154u;
		case KeyCode::LeftControl:	return 0x155u;
		case KeyCode::LeftAlt:		return 0x156u;
		case KeyCode::RightShift:	return 0x158u;
		case KeyCode::RightControl:	return 0x159u;
		case KeyCode::RightAlt:		return 0x15Au;
		case KeyCode::Menu:			return 0x15Cu;
		default:			return 0;
		}
	}
	inline KeyCode GetKeyCodeFromGLFW(int key)
	{
		switch (key)
		{
		case 32: return KeyCode::Space;
		case 39: return KeyCode::Apostrophe;
		case 44: return KeyCode::Comma;
		case 45: return KeyCode::Minus;
		case 46: return KeyCode::Period;
		case 47: return KeyCode::Slash;
		case 48: return KeyCode::D0;
		case 49: return KeyCode::D1;
		case 50: return KeyCode::D2;
		case 51: return KeyCode::D3;
		case 52: return KeyCode::D4;
		case 53: return KeyCode::D5;
		case 54: return KeyCode::D6;
		case 55: return KeyCode::D7;
		case 56: return KeyCode::D8;
		case 57: return KeyCode::D9;
		case 59: return KeyCode::Semicolon;
		case 61: return KeyCode::Equal;
		case 65: return KeyCode::A;
		case 66: return KeyCode::B;
		case 67: return KeyCode::C;
		case 68: return KeyCode::D;
		case 69: return KeyCode::E;
		case 70: return KeyCode::F;
		case 71: return KeyCode::G;
		case 72: return KeyCode::H;
		case 73: return KeyCode::I;
		case 74: return KeyCode::J;
		case 75: return KeyCode::K;
		case 76: return KeyCode::L;
		case 77: return KeyCode::M;
		case 78: return KeyCode::N;
		case 79: return KeyCode::O;
		case 80: return KeyCode::P;
		case 81: return KeyCode::Q;
		case 82: return KeyCode::R;
		case 83: return KeyCode::S;
		case 84: return KeyCode::T;
		case 85: return KeyCode::U;
		case 86: return KeyCode::V;
		case 87: return KeyCode::W;
		case 88: return KeyCode::X;
		case 89: return KeyCode::Y;
		case 90: return KeyCode::Z;
		case 91: return KeyCode::LeftBracket;
		case 92: return KeyCode::Backslash;
		case 93: return KeyCode::RightBracket;
		case 96: return KeyCode::GraveAccent;
		case 256: return KeyCode::Escape;
		case 257: return KeyCode::Enter;
		case 258: return KeyCode::Tab;
		case 259: return KeyCode::Backspace;
		case 260: return KeyCode::Insert;
		case 261: return KeyCode::Delete;
		case 262: return KeyCode::Right;
		case 263: return KeyCode::Left;
		case 264: return KeyCode::Down;
		case 265: return KeyCode::Up;
		case 266: return KeyCode::PageUp;
		case 267: return KeyCode::PageDown;
		case 268: return KeyCode::Home;
		case 269: return KeyCode::End;
		case 280: return KeyCode::CapsLock;
		case 281: return KeyCode::ScrollLock;
		case 282: return KeyCode::NumLock;
		case 283: return KeyCode::PrintScreen;
		case 284: return KeyCode::Pause;
		case 290: return KeyCode::F1;
		case 291: return KeyCode::F2;
		case 292: return KeyCode::F3;
		case 293: return KeyCode::F4;
		case 294: return KeyCode::F5;
		case 295: return KeyCode::F6;
		case 296: return KeyCode::F7;
		case 297: return KeyCode::F8;
		case 298: return KeyCode::F9;
		case 299: return KeyCode::F10;
		case 300: return KeyCode::F11;
		case 301: return KeyCode::F12;
		case 302: return KeyCode::F13;
		case 303: return KeyCode::F14;
		case 304: return KeyCode::F15;
		case 305: return KeyCode::F16;
		case 306: return KeyCode::F17;
		case 307: return KeyCode::F18;
		case 308: return KeyCode::F19;
		case 309: return KeyCode::F20;
		case 310: return KeyCode::F21;
		case 311: return KeyCode::F22;
		case 312: return KeyCode::F23;
		case 313: return KeyCode::F24;
		case 320: return KeyCode::KP0;
		case 321: return KeyCode::KP1;
		case 322: return KeyCode::KP2;
		case 323: return KeyCode::KP3;
		case 324: return KeyCode::KP4;
		case 325: return KeyCode::KP5;
		case 326: return KeyCode::KP6;
		case 327: return KeyCode::KP7;
		case 328: return KeyCode::KP8;
		case 329: return KeyCode::KP9;
		case 330: return KeyCode::KPDecimal;
		case 331: return KeyCode::KPDivide;
		case 332: return KeyCode::KPMultiply;
		case 333: return KeyCode::KPSubtract;
		case 334: return KeyCode::KPAdd;
		case 340: return KeyCode::LeftShift;
		case 341: return KeyCode::LeftControl;
		case 342: return KeyCode::LeftAlt;
		case 344: return KeyCode::RightShift;
		case 345: return KeyCode::RightControl;
		case 346: return KeyCode::RightAlt;
		case 348: return KeyCode::Menu;
		default:  return KeyCode::D0;
		}
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
