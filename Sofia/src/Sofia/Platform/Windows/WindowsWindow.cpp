#include <pch.h>
#include "WindowsWindow.h"

#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/Events/KeyEvents.h"
#include "Sofia/Events/MouseEvents.h"

#include "Sofia/Platform/DX11/DX11Context.h"

#include "Sofia/Application.h"

#include <dxgi1_3.h>

namespace Sofia {

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(m_SwapChain->Present(m_Data.vSync ? 1u : 0u, 0u));
	}

	std::optional<int> WindowsWindow::ProcessEvents()
	{
		MSG msg;
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return {};
	}

	void WindowsWindow::BindToRender() noexcept
	{
		DX11Context::GetContextFromApplication()->GetContext()->OMSetRenderTargets(1u, m_TargetView.GetAddressOf(), nullptr);
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)m_Data.width;
		viewport.Height = (float)m_Data.height;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		DX11Context::GetContextFromApplication()->GetContext()->RSSetViewports(1u, &viewport);
	}
	void WindowsWindow::Clear(const glm::vec4& color) noexcept
	{
		DX11Context::GetContextFromApplication()->GetContext()->ClearRenderTargetView(m_TargetView.Get(), glm::value_ptr(color));
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		if (SetWindowTextA(m_Window, title.c_str()) == 0)
			throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();
		m_Data.title = title;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.title = props.Title;
		m_Data.width = props.Width;
		m_Data.height = props.Height;
		m_Data.eventCallback = SOF_BIND_EVENT_FN(WindowsWindow::DefaultEventCallback);

		SOF_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

		RECT windowArea;
		windowArea.left = 0;
		windowArea.right = m_Data.width + windowArea.left;
		windowArea.top = 0;
		windowArea.bottom = m_Data.height + windowArea.top;
		if (AdjustWindowRect(&windowArea, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, FALSE) == 0)
			throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();

		if (m_Window = CreateWindowExA(0u, WindowClass::GetName(), m_Data.title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, windowArea.right - windowArea.left, windowArea.bottom - windowArea.top, nullptr, nullptr,
			WindowClass::GetInstance(), this); m_Window == nullptr)
			throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();

		ShowWindow(m_Window, SW_SHOW);

		DXGI_SWAP_CHAIN_DESC swapChain = {};
		swapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChain.BufferDesc.Width = m_Data.width;
		swapChain.BufferDesc.Height = m_Data.height;
		swapChain.BufferDesc.RefreshRate.Numerator = 0u;
		swapChain.BufferDesc.RefreshRate.Denominator = 0u;
		swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChain.SampleDesc.Count = 1u;
		swapChain.SampleDesc.Quality = 0u;
		swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChain.BufferCount = 2u;
		swapChain.OutputWindow = m_Window;
		swapChain.Windowed = TRUE;
		swapChain.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChain.Flags = 0u;

		ComPtr<IDXGIDevice3> dxgiDevice;
		ComPtr<IDXGIAdapter> adapter;
		ComPtr<IDXGIFactory> factory;
		DX11Context::GetContextFromApplication()->GetDevice().As(&dxgiDevice);
		HRESULT hr = dxgiDevice->GetAdapter(&adapter);
		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));
			SOF_DX_GRAPHICS_CALL_INFO(factory->CreateSwapChain(DX11Context::GetContextFromApplication()->GetDevice().Get(), &swapChain, &m_SwapChain));
		}

		ComPtr<ID3D11Resource> backBuffer;
		SOF_DX_GRAPHICS_CALL_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
		SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_TargetView));
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(m_Window);
	}

	LRESULT WindowsWindow::HandleMsgSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTA* const create = reinterpret_cast<CREATESTRUCTA*>(lParam);
			WindowsWindow* const window = static_cast<WindowsWindow*>(create->lpCreateParams);

			SetWindowLongPtrA(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

			SetWindowLongPtrA(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::HandleMsgThunk));

			return window->HandleMsg(windowHandle, msg, wParam, lParam);
		}

		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}
	LRESULT WindowsWindow::HandleMsgThunk(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		WindowsWindow* const window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(windowHandle, GWLP_USERDATA));
		return window->HandleMsg(windowHandle, msg, wParam, lParam);
	}
	LRESULT WindowsWindow::HandleMsg(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			m_Data.eventCallback(e);
			PostQuitMessage(0);
			return 0;
		}

		case WM_KILLFOCUS:
		{
			m_Keyboard.ClearState();
			WindowLostFocusEvent e;
			m_Data.eventCallback(e);
			break;
		}
		case WM_SETFOCUS:
		{
			WindowFocusEvent e;
			m_Data.eventCallback(e);
			break;
		}
		case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			m_Data.width = width;
			m_Data.height = height;
			WindowResizeEvent e(width, height);
			m_Data.eventCallback(e);
			break;
		}
		case WM_MOVE:
		{
			int xpos = LOWORD(lParam);
			int ypos = HIWORD(lParam);
			m_Data.pos = { xpos, ypos };
			WindowMovedEvent e(xpos, ypos);
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (!(lParam & 0x40000000) || m_Keyboard.IsAutorepeatEnabled())
				m_Keyboard.OnKeyPressed(static_cast<uint8_t>(wParam));
			KeyPressedEvent e(KeyCode(static_cast<uint8_t>(wParam)), LOWORD(lParam));
			m_Data.eventCallback(e);
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			m_Keyboard.OnKeyReleased(static_cast<uint8_t>(wParam));
			KeyReleasedEvent e(KeyCode(static_cast<uint8_t>(wParam)));
			m_Data.eventCallback(e);
			break;
		}
		case WM_CHAR:
		{
			m_Keyboard.OnChar(static_cast<uint8_t>(wParam));
			KeyTypedEvent e(static_cast<uint8_t>(wParam));
			m_Data.eventCallback(e);
			break;
		}

		case WM_MOUSEMOVE:
		{
			const POINTS points = MAKEPOINTS(lParam);
			MouseMovedEvent e(points.x, points.y);
			if (points.x >= 0 && points.x < (uint16_t)m_Data.width && points.y >= 0 && points.y < (uint16_t)m_Data.height)
			{
				if (!m_Mouse.IsInWindow())
				{
					SetCapture(m_Window);
					m_Mouse.OnMouseEnter();
					MouseEnteredEvent ev;
					m_Data.eventCallback(ev);
				}
				m_Mouse.OnMouseMove(points.x, points.y);
				m_Data.eventCallback(e);
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					m_Mouse.OnMouseMove(points.x, points.y);
					m_Data.eventCallback(e);
				}
				else
				{
					ReleaseCapture();
					m_Mouse.OnMouseLeave();
					MouseLeftEvent ev;
					m_Data.eventCallback(ev);
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS points = MAKEPOINTS(lParam);
			m_Mouse.OnLeftPressed(points.x, points.y);
			MouseButtonPressedEvent e(MouseCode::ButtonLeft);
			m_Data.eventCallback(e);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS points = MAKEPOINTS(lParam);
			m_Mouse.OnLeftReleased(points.x, points.y);
			if (points.x < 0 || points.x >= (uint16_t)m_Data.width || points.y < 0 || points.y >= (uint16_t)m_Data.height)
			{
				ReleaseCapture();
				m_Mouse.OnMouseLeave();
				MouseLeftEvent ev;
				m_Data.eventCallback(ev);
			}
			MouseButtonReleasedEvent e(MouseCode::ButtonLeft);
			m_Data.eventCallback(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS points = MAKEPOINTS(lParam);
			m_Mouse.OnRightPressed(points.x, points.y);
			MouseButtonPressedEvent e(MouseCode::ButtonRight);
			m_Data.eventCallback(e);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS points = MAKEPOINTS(lParam);
			m_Mouse.OnRightReleased(points.x, points.y);
			if (points.x < 0 || points.x >= (uint16_t)m_Data.width || points.y < 0 || points.y >= (uint16_t)m_Data.height)
			{
				ReleaseCapture();
				m_Mouse.OnMouseLeave();
				MouseLeftEvent ev;
				m_Data.eventCallback(ev);
			}
			MouseButtonReleasedEvent e(MouseCode::ButtonRight);
			m_Data.eventCallback(e);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			const POINTS points = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			m_Mouse.OnWheelDelta(points.x, points.y, delta);
			MouseScrolledEvent e(0, (float)delta);
			m_Data.eventCallback(e);
			break;
		}
		}
		return DefWindowProcA(windowHandle, msg, wParam, lParam);
	}



	WindowsWindow::WindowClass WindowsWindow::WindowClass::s_WindowClass;
	WindowsWindow::WindowClass::WindowClass() noexcept
		: m_Instance(GetModuleHandle(nullptr))
	{
		WNDCLASSEXA windowClass = { 0 };
		windowClass.cbSize = sizeof(WNDCLASSEXA);
		windowClass.style = CS_OWNDC;
		windowClass.lpfnWndProc = WindowsWindow::HandleMsgSetup;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = m_Instance;
		windowClass.hIcon = nullptr;
		windowClass.hCursor = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = GetName();
		windowClass.hIconSm = nullptr;

		RegisterClassExA(&windowClass);
	}
	WindowsWindow::WindowClass::~WindowClass()
	{
		UnregisterClassA(GetName(), m_Instance);
	}




	WindowsWindow::WindowException::WindowException(int line, const char* file, HRESULT hresult) noexcept
		: SofiaException(line, file, true), m_Result(hresult) {}

	const char* WindowsWindow::WindowException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl;
		oss << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (uint32_t)GetErrorCode() << ")" << std::endl;
		oss << "[Description] " << GetErrorDescription() << std::endl;
		oss << GetOriginString();
		m_Buffer = oss.str();
		return m_Buffer.c_str();
	}
	std::string WindowsWindow::WindowException::GetErrorDescription() const noexcept
	{
		return TranslateErrorCode(m_Result);
	}
	std::string WindowsWindow::WindowException::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* msgBuffer = nullptr;

		const uint32_t msgLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuffer), 0, nullptr);

		if (msgLength == 0)
			return "Unidentified error code";

		std::string errorString = msgBuffer;
		LocalFree(msgBuffer);
		return errorString;
	}
}
