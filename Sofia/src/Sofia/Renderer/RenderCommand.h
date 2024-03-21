#pragma once

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	class RenderCommand
	{
		friend class Renderer;
	public:
		SOF_CORE static void Draw(RendererAPI::Topology topology, uint32_t verticesCount);
		SOF_CORE static void DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount);
	private:
		static void Init();
		static void Shutdown();
	private:
		SOF_CORE static Scope<RendererAPI> s_API;
	};
}
