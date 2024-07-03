#pragma once

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	class RenderCommand
	{
		friend class Renderer;
	public:
		SOF_CORE static void Draw(RendererAPI::Topology topology, uint32_t verticesCount);
		SOF_CORE static void DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount);

		SOF_CORE static void SetDepthTestOptions(bool enable, bool writeMask = true, RendererAPI::ComparisonFunc compFunc = RendererAPI::ComparisonFunc::Less);
		SOF_CORE static void SetStencilTestOptions(bool enable, uint8_t writeMask = 0xffu, uint8_t readMask = 0xffu,
			RendererAPI::ComparisonFunc frontFaceFunc = RendererAPI::ComparisonFunc::Always,
			RendererAPI::ComparisonFunc backFaceFunc = RendererAPI::ComparisonFunc::Always, uint32_t stencilRefVal = 0u);
		SOF_CORE static void SetFrontFaceStencilOperations(RendererAPI::StencilOperation stencilFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation depthFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation pass = RendererAPI::StencilOperation::Keep);
		SOF_CORE static void SetBackFaceStencilOperations(RendererAPI::StencilOperation stencilFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation depthFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation pass = RendererAPI::StencilOperation::Keep);
		SOF_CORE static void SetBlendOptions(uint32_t i, bool enable, RendererAPI::BlendOption sourceBlend = RendererAPI::BlendOption::One,
			RendererAPI::BlendOption destinationBlend = RendererAPI::BlendOption::Zero, RendererAPI::BlendOperation operation = RendererAPI::BlendOperation::Add,
			RendererAPI::BlendOption sourceAlphaBlend = RendererAPI::BlendOption::One, RendererAPI::BlendOption destinationAlphaBlend = RendererAPI::BlendOption::Zero,
			RendererAPI::BlendOperation alphaOperation = RendererAPI::BlendOperation::Add, uint8_t writeMask = 1 | 2 | 4 | 8, glm::vec4 blendFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	private:
		static void Init();
		static void Shutdown();
	private:
		SOF_CORE static Scope<RendererAPI> s_API;
	};
}
