#pragma once

namespace Sofia {

	class RendererAPI
	{
		friend class Application;
	public:
		enum class API
		{
			None = 0, OpenGL, DX11, DX12, Vulkan
		};
	public:
		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
