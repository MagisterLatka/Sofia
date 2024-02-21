#include <pch.h>
#include "Window.h"

#include "Sofia/Renderer/RendererAPI.h"
#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/Windows/WindowsWindow.h"
#else
#	include "Sofia/Platform/Linux/LinuxWindow.h"
#endif

namespace Sofia {

	Ref<Window> Window::Create(const WindowProps& props)
	{
#if defined(SOF_PLATFORM_WINDOWS)
		return Ref<WindowsWindow>::Create(props);
#else
		return Ref<LinuxWindow>::Create(props);
#endif
	}
	std::optional<int> Window::ProcessEvents()
	{
#if defined(SOF_PLATFORM_WINDOWS)
		return WindowsWindow::ProcessEvents();
#else
		return LinuxWindow::ProcessEvents();
#endif
	}
}
