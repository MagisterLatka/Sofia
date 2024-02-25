#pragma once

#include "Sofia/Renderer/RenderTarget.h"
#include <d3d11.h>

namespace Sofia {

	class DX11RenderTarget : public RenderTarget
	{
		friend class DX11RenderPass;
	public:
		SOF_CORE DX11RenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format);
		SOF_CORE virtual ~DX11RenderTarget() = default;

		SOF_CORE virtual void Bind() const noexcept override;
		SOF_CORE virtual void Clear(const glm::vec4& clearVal) noexcept override;
		SOF_CORE virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

		SOF_CORE virtual void BindTexture(uint32_t slot = 0u) const noexcept override;
		SOF_CORE virtual void* GetRawTexturePointer() const noexcept override { return m_View.Get(); }
	private:
		ComPtr<ID3D11RenderTargetView> m_RenderTarget;
		ComPtr<ID3D11ShaderResourceView> m_View;
		uint32_t m_Width = 0, m_Height = 0;
		RenderTargetFormat m_Format;
	};
}