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

		SOF_CORE virtual void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
		SOF_CORE virtual void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc, ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
		SOF_CORE virtual void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		SOF_CORE virtual void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		SOF_CORE virtual void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
		SOF_CORE virtual void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend,
			BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor) override;
	private:
		void InitShaders();
		void SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology);
		void RecreateDepthStencilState();
	private:
		ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc;
		uint32_t m_StencilRefVal = 0u;

		ComPtr<ID3D11RasterizerState> m_RasterizerState;
		ComPtr<ID3D11BlendState> m_BlendState;
		D3D11_BLEND_DESC m_BlendDesc;
	};
}
