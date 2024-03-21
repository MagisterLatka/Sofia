#pragma once

#include "Sofia/Renderer/Texture.h"
#include <d3d11.h>

namespace Sofia {

	class DX11Texture2D : public Texture2D
	{
	public:
		SOF_CORE DX11Texture2D(const Texture2DProps& props);
		SOF_CORE DX11Texture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
		SOF_CORE virtual ~DX11Texture2D() = default;

		SOF_CORE virtual void Bind(uint32_t slot) const noexcept override;

		SOF_CORE virtual const Texture2DProps& GetProps() const noexcept override { return m_Props; }
		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Props.Width; }
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Props.Height; }
		SOF_CORE virtual void* GetRawPointer() const noexcept override { return m_View.Get(); }
	private:
		void Init();
	private:
		ComPtr<ID3D11ShaderResourceView> m_View;
		ComPtr<ID3D11SamplerState> m_Sampler;
		Texture2DProps m_Props;

		Sofia::Buffer m_Buffer;
	};
}
