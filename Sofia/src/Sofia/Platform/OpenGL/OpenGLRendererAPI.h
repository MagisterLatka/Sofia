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

		SOF_CORE virtual void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
		SOF_CORE virtual void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc, ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
		SOF_CORE virtual void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		SOF_CORE virtual void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
		SOF_CORE virtual void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
		SOF_CORE virtual void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend,
			BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor) override;
	private:
		int GetTopology(Topology topology);
		void InitShaders();
	};
}
	