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

		virtual void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
		virtual void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc, ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
		virtual void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		virtual void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		virtual void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
		virtual void SetBlendOptions(bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend,
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
	};
}
