#pragma once

#include "Sofia/Renderer/RenderTarget.h"

namespace Sofia {

	class OpenGLRenderTarget : public RenderTarget
	{
		friend class OpenGLRenderPass;
	public:
		SOF_CORE OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, const glm::vec4& clearVal);
		SOF_CORE virtual ~OpenGLRenderTarget();

		SOF_CORE virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

		SOF_CORE virtual void BindTexture(uint32_t slot = 0u) const noexcept override;
		SOF_CORE virtual void* GetRawTexturePointer() const noexcept override { return (void*)(uint64_t)m_ID; }
		SOF_CORE virtual void ReadPixel(void* data, uint32_t xCoord, uint32_t yCoord) override;

		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Width; };
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Height; };
	private:
		virtual void SetClearValue(const glm::vec4& clearVal) noexcept override { m_ClearValue = clearVal; }
		virtual void SetDepthStencilClearValue(float depth, uint8_t stencil) noexcept override { m_DepthClearValue = depth; m_StencilClearValue = stencil; };
		virtual void Clear() noexcept override {}
	private:
		uint32_t m_ID = 0, m_Width = 0, m_Height = 0;
		RenderTargetFormat m_Format;
		glm::vec4 m_ClearValue;
		float m_DepthClearValue = 1.0f;
		uint8_t m_StencilClearValue = 0u;
	};
}
