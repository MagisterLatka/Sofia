#include <pch.h>
#include "Window.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Platform/Windows/WindowsWindow.h"

namespace Sofia {

	Ref<Window> Window::Create(const WindowProps& props)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	return Ref<WindowsWindow>::Create(props);
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	std::optional<int> Window::ProcessEvents()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return {};
		case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return {};
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return {};
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	return WindowsWindow::ProcessEvents();
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return {};
	}
}
