#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class RendererAPI
	{
		friend class Application;
		friend class RenderCommand;
	public:
		enum class API
		{
			None = 0, OpenGL, DX11, DX12, Vulkan
		};
		enum class Topology
		{
			None = 0, Points, Lines, LineStrip, Triangles, TriangleStrip
		};
	public:
		SOF_CORE static API GetAPI() { return s_API; }
	public:
		SOF_CORE virtual ~RendererAPI() = default;

		SOF_CORE virtual void Init() = 0;
		SOF_CORE virtual void Shutdown() = 0;

		SOF_CORE virtual void Draw(Topology topology, uint32_t verticesCount) = 0;
		SOF_CORE virtual void DrawIndexed(Topology topology, uint32_t indicesCount) = 0;
	private:
		SOF_CORE static Scope<RendererAPI> Create();
	private:
		SOF_CORE static API s_API;
	};
}
