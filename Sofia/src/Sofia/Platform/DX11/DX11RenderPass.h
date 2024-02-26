#pragma once

#include "Sofia/Renderer/RenderPass.h"
#include "Sofia/Platform/DX11/DX11RenderTarget.h"

namespace Sofia {

	class DX11RenderPass : public RenderPass
	{
	public:
		SOF_CORE DX11RenderPass() = default;
		SOF_CORE virtual ~DX11RenderPass() = default;

		SOF_CORE virtual void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
		SOF_CORE virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
		SOF_CORE virtual Ref<RenderTarget> GetRenderTarget(uint32_t index) const override
		{
			SOF_CORE_ASSERT(index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Exceeding available number of simultaneous render targets");
			return m_RenderTargets[index];
		}
		SOF_CORE virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

		SOF_CORE virtual void Bind() const noexcept override;
		SOF_CORE virtual void Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept override;
		SOF_CORE virtual void Resize(uint32_t width, uint32_t height) override;
	private:
		Ref<DX11RenderTarget> m_RenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		Ref<DX11RenderTarget> m_DepthStencilTarget;

		uint32_t m_Width = 0u, m_Height = 0u;
	};
}
