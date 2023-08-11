#pragma once

#include "Sofia/Renderer/RendererAPI.h"
#include <d3d11.h>

namespace Sofia {

	class DX11RendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void Draw(Topology topology, uint32_t verticesCount) override;
		virtual void DrawIndexed(Topology topology, uint32_t indicesCount) override;
	private:
		void SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology);
	};
}
