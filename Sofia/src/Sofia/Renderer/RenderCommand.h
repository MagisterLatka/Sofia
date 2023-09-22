#pragma once

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	class RenderCommand
	{
		friend class Renderer;
	public:
		static void Draw(RendererAPI::Topology topology, uint32_t verticesCount);
		static void DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount);

		static void SetDepthTestOptions(bool enable, bool writeMask = true, RendererAPI::ComparisonFunc compFunc = RendererAPI::ComparisonFunc::Less);
		static void SetStencilTestOptions(bool enable, uint8_t writeMask = 0xffu, uint8_t readMask = 0xffu,
			RendererAPI::ComparisonFunc frontFaceFunc = RendererAPI::ComparisonFunc::Always,
			RendererAPI::ComparisonFunc backFaceFunc = RendererAPI::ComparisonFunc::Always, uint32_t stencilRefVal = 0u);
		static void SetFrontFaceStencilOperations(RendererAPI::StencilOperation stencilFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation depthFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation pass = RendererAPI::StencilOperation::Keep);
		static void SetBackFaceStencilOperations(RendererAPI::StencilOperation stencilFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation depthFail = RendererAPI::StencilOperation::Keep,
			RendererAPI::StencilOperation pass = RendererAPI::StencilOperation::Keep);
		static void SetBlendOptions(bool enable, RendererAPI::BlendOption sourceBlend = RendererAPI::BlendOption::One,
			RendererAPI::BlendOption destinationBlend = RendererAPI::BlendOption::Zero, RendererAPI::BlendOperation operation = RendererAPI::BlendOperation::Add,
			RendererAPI::BlendOption sourceAlphaBlend = RendererAPI::BlendOption::One, RendererAPI::BlendOption destinationAlphaBlend = RendererAPI::BlendOption::Zero,
			RendererAPI::BlendOperation alphaOperation = RendererAPI::BlendOperation::Add, uint8_t writeMask = 1 | 2 | 4 | 8, glm::vec4 blendFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	private:
		static void Init();
	private:
		static Scope<RendererAPI> s_API;
	};
}
