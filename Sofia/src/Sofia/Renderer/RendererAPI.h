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
		static API GetAPI() { return s_API; }
	public:
		virtual void Init() = 0;

		virtual void Draw(Topology topology, uint32_t verticesCount) = 0;
		virtual void DrawIndexed(Topology topology, uint32_t indicesCount) = 0;
	private:
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};
}
