#include <pch.h>
#include "InputLayout.h"

#include "Sofia/Renderer/RendererAPI.h"
#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/DX11/DX11InputLayout.h"
#endif

#include "Sofia/Platform/OpenGL/OpenGLInputLayout.h"

namespace Sofia {

	Ref<InputLayout> InputLayout::Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<OpenGLInputLayout>::Create(vertexBuffers, shader, indexBuffer);
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
		case RendererAPI::API::DX11:	return Ref<DX11InputLayout>::Create(vertexBuffers, shader, indexBuffer);
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
}
