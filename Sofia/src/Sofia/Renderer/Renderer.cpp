#include <pch.h>
#include "Renderer.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Renderer/RenderCommand.h"
#include "Sofia/Renderer/Renderer2D.h"

namespace Sofia {

	struct RendererData
	{
		RenderCommandQueue* commandQueue;
		ShaderLibrary* shaderLibrary;
	};
	static RendererData s_RendererData;

	void Renderer::Init()
	{
		s_RendererData.commandQueue = new RenderCommandQueue;
		s_RendererData.shaderLibrary = new ShaderLibrary;
		RenderCommand::Init();
		s_RendererData.commandQueue->Execute();
		Renderer2D::Init();
	}
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		delete s_RendererData.shaderLibrary;
		s_RendererData.commandQueue->Execute();
		delete s_RendererData.commandQueue;
	}

	void Renderer::Render()
	{
		s_RendererData.commandQueue->Execute();
	}
	ShaderLibrary& Renderer::GetShaderLibrary() noexcept
	{
		return *s_RendererData.shaderLibrary;
	}
	RenderCommandQueue& Renderer::GetRenderCommandQueue() noexcept
	{
		return *s_RendererData.commandQueue;
	}
}
