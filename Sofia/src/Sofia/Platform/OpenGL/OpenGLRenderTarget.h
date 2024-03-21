#pragma once

#include "Sofia/Renderer/RenderTarget.h"

namespace Sofia {

	class OpenGLRenderTarget : public RenderTarget
	{
		friend class OpenGLRenderPass;
	public:
		SOF_CORE OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format);
		SOF_CORE virtual ~OpenGLRenderTarget();

		SOF_CORE virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

		SOF_CORE virtual void BindTexture(uint32_t slot = 0u) const noexcept override;
		SOF_CORE virtual void* GetRawTexturePointer() const noexcept override { return (void*)(uint64_t)m_ID; }

		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Width; };
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Height; };
	private:
		virtual void Clear(const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), float depth = 0.0f, uint8_t stencil = 0u) noexcept override {}
	private:
		uint32_t m_ID = 0, m_Width = 0, m_Height = 0;
		RenderTargetFormat m_Format;
	};
}
