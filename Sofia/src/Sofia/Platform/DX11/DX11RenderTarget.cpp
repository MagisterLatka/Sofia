#include <pch.h>
#include "DX11RenderTarget.h"

#include "Sofia/Renderer/Renderer.h"
#include "Sofia/Platform/DX11/DX11Context.h"

namespace Sofia {

	static DXGI_FORMAT GetFormat(RenderTargetFormat format)
	{
		switch (format)
		{
		case Sofia::RenderTargetFormat::R8:			return DXGI_FORMAT_R8_UNORM;
		case Sofia::RenderTargetFormat::R32F:		return DXGI_FORMAT_R32_FLOAT;
		case Sofia::RenderTargetFormat::RG8:		return DXGI_FORMAT_R8G8_UNORM;
		case Sofia::RenderTargetFormat::RG16F:		return DXGI_FORMAT_R16G16_FLOAT;
		case Sofia::RenderTargetFormat::RG32F:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case Sofia::RenderTargetFormat::RGB8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Sofia::RenderTargetFormat::RGB32F:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case Sofia::RenderTargetFormat::RGBA8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Sofia::RenderTargetFormat::RGBA16F:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case Sofia::RenderTargetFormat::RGBA32F:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		SOF_CORE_THROW_INFO("Unknown render target format");
		return DXGI_FORMAT_R8G8B8A8_UINT;
	}
	static uint32_t GetBPP(RenderTargetFormat format)
	{
		switch (format)
		{
		case Sofia::RenderTargetFormat::R8:
			return 1;
		case Sofia::RenderTargetFormat::R32F:
		case Sofia::RenderTargetFormat::RG16F:
		case Sofia::RenderTargetFormat::RGBA8:
			return 4;
		case Sofia::RenderTargetFormat::RG8:
			return 2;
		case Sofia::RenderTargetFormat::RG32F:
		case Sofia::RenderTargetFormat::RGBA16F:
			return 8;
		case Sofia::RenderTargetFormat::RGB8:
			return 3;
		case Sofia::RenderTargetFormat::RGB32F:
			return 12;
		case Sofia::RenderTargetFormat::RGBA32F:
			return 16;
		}
		SOF_CORE_THROW_INFO("Unknown render target format");
		return 0;
	}

	DX11RenderTarget::DX11RenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format)
		: m_Width(width), m_Height(height), m_Format(format)
	{
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
		SOF_CORE_ASSERT((uint32_t)format > 0 && (uint32_t)format <= (uint32_t)RenderTargetFormat::Last, "Unknown render target format");
		Resize(m_Width, m_Height, true);
	}
	void DX11RenderTarget::Resize(uint32_t width, uint32_t height, bool forceResize)
	{
		if (m_Width == width && m_Height == height && !forceResize) return;
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
		m_Width = width;
		m_Height = height;

		Ref<DX11RenderTarget> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = DX11Context::GetContextFromApplication()->GetDevice();
			HRESULT hr;

			D3D11_TEXTURE2D_DESC textureDesc = { 0 };
			textureDesc.Width = instance->m_Width;
			textureDesc.Height = instance->m_Height;
			textureDesc.MipLevels = 1u;
			textureDesc.ArraySize = 1u;
			textureDesc.Format = GetFormat(instance->m_Format);
			textureDesc.SampleDesc.Count = 1u;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

			ComPtr<ID3D11Texture2D> texture;
			SOF_DX_GRAPHICS_CALL_INFO(device->CreateTexture2D(&textureDesc, nullptr, &texture));

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
			renderTargetDesc.Format = textureDesc.Format;
			renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice = 0u;

			SOF_DX_GRAPHICS_CALL_INFO(device->CreateRenderTargetView(texture.Get(), &renderTargetDesc, &instance->m_RenderTarget));

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
			shaderResourceDesc.Format = textureDesc.Format;
			shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceDesc.Texture2D.MostDetailedMip = 0u;
			shaderResourceDesc.Texture2D.MipLevels = 1u;

			SOF_DX_GRAPHICS_CALL_INFO(device->CreateShaderResourceView(texture.Get(), &shaderResourceDesc, &instance->m_View));
		});
	}

	void DX11RenderTarget::BindTexture(uint32_t slot) const noexcept
	{
		Ref<const DX11RenderTarget> instance = this;
		Renderer::Submit([instance, slot]()
		{
			DX11Context::GetContextFromApplication()->GetContext()->PSSetShaderResources(slot, 1u, instance->m_View.GetAddressOf());
		});
	}

	void DX11RenderTarget::Bind() const noexcept
	{
		Ref<const DX11RenderTarget> instance = this;
		Renderer::Submit([instance]()
		{
			DX11Context::GetContextFromApplication()->GetContext()->OMSetRenderTargets(1u, instance->m_RenderTarget.GetAddressOf(), nullptr);
			D3D11_VIEWPORT viewport;
			viewport.Width = (float)instance->m_Width;
			viewport.Height = (float)instance->m_Height;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			DX11Context::GetContextFromApplication()->GetContext()->RSSetViewports(1u, &viewport);
		});
	}

	void DX11RenderTarget::Clear(const glm::vec4& clearVal) noexcept
	{
		Ref<DX11RenderTarget> instance = this;
		Renderer::Submit([instance, clearVal]()
		{
			DX11Context::GetContextFromApplication()->GetContext()->ClearRenderTargetView(instance->m_RenderTarget.Get(), glm::value_ptr(clearVal));
		});
	}
}
