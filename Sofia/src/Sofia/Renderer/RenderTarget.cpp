#include <pch.h>
#include "RenderTarget.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Platform/DX11/DX11RenderTarget.h"

namespace Sofia {

	Ref<RenderTarget> RenderTarget::Create(uint32_t width, uint32_t height, RenderTargetFormat format, const glm::vec4& clearVal)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
		case RendererAPI::API::DX11:	return Ref<DX11RenderTarget>::Create(width, height, format, clearVal);
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
}
