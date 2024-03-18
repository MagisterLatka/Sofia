#pragma once

#include "Sofia/Renderer/RendererAPI.h"
#include <d3d11.h>

namespace Sofia {

	class DX11RendererAPI : public RendererAPI
	{
	public:
		SOF_CORE virtual void Init() override;
		SOF_CORE virtual void Shutdown() override;

		SOF_CORE virtual void Draw(Topology topology, uint32_t verticesCount) override;
		SOF_CORE virtual void DrawIndexed(Topology topology, uint32_t indicesCount) override;
	private:
		SOF_CORE void InitShaders();
		SOF_CORE void SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology);
	};
}
