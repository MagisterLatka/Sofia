#pragma once

#include "Sofia/Renderer/RenderPass.h"
#include "Sofia/Platform/DX11/DX11RenderTarget.h"

namespace Sofia {

	class DX11RenderPass : public RenderPass
	{
	public:
		DX11RenderPass() = default;
		virtual ~DX11RenderPass() = default;

		virtual void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
		virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
		virtual Ref<RenderTarget> GetRenderTarget(uint32_t index) const override
		{
			SOF_CORE_ASSERT(index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Exceeding available number of simultaneous render targets");
			return m_RenderTargets[index];
		}
		virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

		virtual void Bind() const noexcept override;
		virtual void Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept override;
		virtual void Resize(uint32_t width, uint32_t height) override;
	private:
		Ref<DX11RenderTarget> m_RenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		Ref<DX11RenderTarget> m_DepthStencilTarget;

		uint32_t m_Width = 0u, m_Height = 0u;
	};
}
