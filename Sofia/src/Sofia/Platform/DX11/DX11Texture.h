#pragma once

#include "Sofia/Renderer/Texture.h"
#include <d3d11.h>

namespace Sofia {

	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(const Texture2DProps& props);
		DX11Texture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
		virtual ~DX11Texture2D() = default;

		virtual void Bind(uint32_t slot) const noexcept override;

		virtual const Texture2DProps& GetProps() const noexcept override { return m_Props; }
		virtual uint32_t GetWidth() const noexcept override { return m_Props.Width; }
		virtual uint32_t GetHeight() const noexcept override { return m_Props.Height; }
		virtual void* GetRawPointer() const noexcept override { return m_View.Get(); }
	private:
		void Init();
	private:
		ComPtr<ID3D11ShaderResourceView> m_View;
		ComPtr<ID3D11SamplerState> m_Sampler;
		Texture2DProps m_Props;

		Sofia::Buffer m_Buffer;
	};
}
