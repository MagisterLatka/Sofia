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
	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.commandQueue = new RenderCommandQueue;
		RenderCommand::Init();
		s_Data.shaderLibrary = new ShaderLibrary;
		Renderer2D::Init();
		s_Data.commandQueue->Execute();
	}
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		delete s_Data.shaderLibrary;
		RenderCommand::Shutdown();
		s_Data.commandQueue->Execute();
		delete s_Data.commandQueue;
	}

	void Renderer::Render()
	{
		s_Data.commandQueue->Execute();
	}
	ShaderLibrary& Renderer::GetShaderLibrary() noexcept
	{
		return *s_Data.shaderLibrary;
	}
	RenderCommandQueue& Renderer::GetRenderCommandQueue() noexcept
	{
		return *s_Data.commandQueue;
	}
}
