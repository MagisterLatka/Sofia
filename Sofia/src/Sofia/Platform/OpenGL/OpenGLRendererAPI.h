#pragma once

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		SOF_CORE virtual void Init() override;
		SOF_CORE virtual void Shutdown() override;

		SOF_CORE virtual void Draw(Topology topology, uint32_t verticesCount) override;
		SOF_CORE virtual void DrawIndexed(Topology topology, uint32_t indicesCount) override;
	private:
		SOF_CORE int GetTopology(Topology topology);
	};
}
	