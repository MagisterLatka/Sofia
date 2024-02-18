#pragma once

#include "Sofia/Core.h"

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
		SOF_CORE static API GetAPI() { return s_API; }
	private:
		SOF_CORE static API s_API;
	};
}
