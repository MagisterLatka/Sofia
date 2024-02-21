#include <pch.h>
#include "GraphicsContext.h"

#include "Sofia/Renderer/RendererAPI.h"

#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/DX11/DX11Context.h"
#endif

#include "Sofia/Platform/OpenGL/OpenGLContext.h"

namespace Sofia {

	Ref<GraphicsContext> GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<OpenGLContext>::Create();
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SOF_PLATFORM_WINDOWS)
		case RendererAPI::API::DX11:	return Ref<DX11Context>::Create();
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX12 is not supported yet"); return nullptr;
#else
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
}
