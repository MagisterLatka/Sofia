#pragma once

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	class RenderCommand
	{
		friend class Renderer;
	public:
		static void Draw(RendererAPI::Topology topology, uint32_t verticesCount);
		static void DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount);
	private:
		static void Init();
	private:
		static Scope<RendererAPI> s_API;
	};
}
