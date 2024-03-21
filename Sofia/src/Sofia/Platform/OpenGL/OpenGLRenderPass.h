#pragma once

#include "Sofia/Renderer/RenderPass.h"
#include "Sofia/Platform/OpenGL/OpenGLRenderTarget.h"

namespace Sofia {

	class OpenGLRenderPass : public RenderPass
	{
	public:
		SOF_CORE OpenGLRenderPass();
		SOF_CORE virtual ~OpenGLRenderPass();

		SOF_CORE virtual void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
		SOF_CORE virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
		SOF_CORE virtual Ref<RenderTarget> GetRenderTarget(uint32_t index) const override
		{
			SOF_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
			return m_RenderTargets[index];
		}
		SOF_CORE virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

		SOF_CORE virtual void Bind() const noexcept override;
		SOF_CORE virtual void Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept override;
		SOF_CORE virtual void Resize(uint32_t width, uint32_t height) override;

		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Width; };
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Height; };
	private:
		void Create();
	private:
		Ref<OpenGLRenderTarget> m_RenderTargets[8];
		Ref<OpenGLRenderTarget> m_DepthStencilTarget;

		uint32_t m_ID = 0u, m_Width = 0u, m_Height = 0u;
	};
}
