#include <pch.h>
#include "RenderCommand.h"

#include "Sofia/Renderer/Renderer.h"
#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/DX11/DX11RendererAPI.h"
#endif

#include "Sofia/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Sofia {

	Scope<RendererAPI> RenderCommand::s_API;
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
		case RendererAPI::API::DX11:	return CreateScope<DX11RendererAPI>();
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}

	void RenderCommand::Init()
	{
		s_API = RendererAPI::Create();
		Renderer::Submit([]() { s_API->Init(); });
	}
	void RenderCommand::Shutdown()
	{
		Renderer::Submit([]() { s_API->Shutdown(); });
	}

	void RenderCommand::Draw(RendererAPI::Topology topology, uint32_t verticesCount)
	{
		Renderer::Submit([topology, verticesCount]() { s_API->Draw(topology, verticesCount); });
	}
	void RenderCommand::DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount)
	{
		Renderer::Submit([topology, indicesCount]() { s_API->DrawIndexed(topology, indicesCount); });
	}
}
