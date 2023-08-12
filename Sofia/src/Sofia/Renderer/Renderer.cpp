#include <pch.h>
#include "Renderer.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Renderer/RenderCommand.h"

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
		s_Data.commandQueue->Execute();
		s_Data.shaderLibrary = new ShaderLibrary;
	}
	void Renderer::Shutdown()
	{
		delete s_Data.shaderLibrary;
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
