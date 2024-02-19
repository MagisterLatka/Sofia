#include <pch.h>
#include "Window.h"

#include "Sofia/Renderer/RendererAPI.h"
#if defined(SOF_PLATFORM_WINDOWS)
#include "Sofia/Platform/Windows/WindowsWindow.h"
#endif
#include "Sofia/Platform/Linux/LinuxWindow.h"

namespace Sofia {

	Ref<Window> Window::Create(const WindowProps& props)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<LinuxWindow>::Create(props);
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SOF_PLATFORM_WINDOWS)
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	return Ref<WindowsWindow>::Create(props);
#else
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	std::optional<int> Window::ProcessEvents()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return {};
		case RendererAPI::API::OpenGL:	return LinuxWindow::ProcessEvents();
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return {};
#if defined(SOF_PLATFORM_WINDOWS)
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	return WindowsWindow::ProcessEvents();
#else
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return {};
#endif
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return {};
	}
}
