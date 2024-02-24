#include <pch.h>
#include "Texture.h"

#include "Sofia/Renderer/RendererAPI.h"
#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/DX11/DX11Texture.h"
#endif

#include "Sofia/Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Sofia {

	Ref<Texture2D> Texture2D::Create(const Texture2DProps& props)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<OpenGLTexture2D>::Create(props);
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SOF_PLATFORM_WINDOWS)
		case RendererAPI::API::DX11:	return Ref<DX11Texture2D>::Create(props);
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data, TextureFormat format)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<OpenGLTexture2D>::Create(width, height, data, format);
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SOF_PLATFORM_WINDOWS)
		case RendererAPI::API::DX11:	return Ref<DX11Texture2D>::Create(width, height, data, format);
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}

	uint32_t Texture2D::GetBPP(TextureFormat format)
	{
		switch (format)
		{
		case Sofia::TextureFormat::R8:		return 1 * sizeof(uint8_t);
		case Sofia::TextureFormat::R32F:	return 1 * sizeof(float);
		case Sofia::TextureFormat::RGB8:	return 3 * sizeof(uint8_t);
		case Sofia::TextureFormat::RGB32F:	return 3 * sizeof(float);
		case Sofia::TextureFormat::RGBA8:	return 4 * sizeof(uint8_t);
		case Sofia::TextureFormat::RGBA32F:	return 4 * sizeof(float);
		}
		SOF_CORE_THROW_INFO("Unknown texture format");
		return 0;
	}
	uint32_t Texture2D::CalcNrMips(uint32_t width, uint32_t height)
	{
		uint32_t levels = 1;
		while ((width | height) >> levels)
			++levels;

		return levels;
	}
	void* Texture2D::Decode(void* const data, uint32_t size, uint32_t& width, uint32_t& height)
	{
		int x, y, channels;
		void* output = stbi_load_from_memory((uint8_t*)data, size, &x, &y, &channels, 4);
		width = x;
		height = y;
		return output;
	}
}
