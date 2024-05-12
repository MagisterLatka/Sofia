#pragma once

#include "Sofia/Renderer/RenderTarget.h"

namespace Sofia {

	class RenderPass : public RefCounted
	{
	public:
		SOF_CORE virtual ~RenderPass() = default;

		SOF_CORE virtual void SetRenderTarget(uint32_t index = 0, Ref<RenderTarget> renderTarget = {}) = 0;
		SOF_CORE virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget = {}) = 0;
		SOF_CORE virtual Ref<RenderTarget> GetRenderTarget(uint32_t index = 0) const = 0;
		SOF_CORE virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept = 0;

		SOF_CORE virtual uint32_t GetWidth() const noexcept = 0;
		SOF_CORE virtual uint32_t GetHeight() const noexcept = 0;

		SOF_CORE virtual void Bind() const noexcept = 0;
		SOF_CORE virtual void Clear() noexcept = 0;
		SOF_CORE virtual void Resize(uint32_t width, uint32_t height) = 0;

		SOF_CORE static Ref<RenderPass> Create();
	};
}
