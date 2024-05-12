#include <pch.h>
#include "OpenGLRenderTarget.h"

#include "Sofia/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Sofia {

	static GLenum GetFormat(RenderTargetFormat format)
	{
		switch (format)
		{
		case RenderTargetFormat::R8:			return GL_R8;
		case RenderTargetFormat::R32F:			return GL_R32F;
		case RenderTargetFormat::R32_UINT:		return GL_R32UI;
		case RenderTargetFormat::RG8:			return GL_RG8;
		case RenderTargetFormat::RG16F:			return GL_RG16F;
		case RenderTargetFormat::RG32F:			return GL_RG32F;
		case RenderTargetFormat::RG32_UINT:		return GL_RG32UI;
		case RenderTargetFormat::RGB8:			return GL_RGB8;
		case RenderTargetFormat::RGB32F:		return GL_RGB32F;
		case RenderTargetFormat::RGB32_UINT:	return GL_RGB32UI;
		case RenderTargetFormat::RGBA8:			return GL_RGBA8;
		case RenderTargetFormat::RGBA16F:		return GL_RGBA16F;
		case RenderTargetFormat::RGBA32F:		return GL_RGBA32F;
		case RenderTargetFormat::RGBA32_UINT:	return GL_RGBA32UI;
		}
		SOF_CORE_THROW_INFO("Unknown render target format");
		return 0;
	}
	OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, const glm::vec4& clearVal)
		: m_Width(width), m_Height(height), m_Format(format), m_ClearValue(clearVal)
	{
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
		SOF_CORE_ASSERT((uint32_t)format > 0 && (uint32_t)format <= (uint32_t)RenderTargetFormat::Last, "Unknown render target format");
		Resize(m_Width, m_Height, true);
	}
	OpenGLRenderTarget::~OpenGLRenderTarget()
	{
		glDeleteTextures(1, &m_ID);
	}
	void OpenGLRenderTarget::Resize(uint32_t width, uint32_t height, bool forceResize) //TODO: depth buffer
	{
		if (m_Width == width && m_Height == height && !forceResize) return;
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
		m_Width = width;
		m_Height = height;

		Ref<OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance]() mutable
		{
			if (instance->m_ID)
			{
				glDeleteTextures(1, &instance->m_ID);
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &instance->m_ID);
			glTextureStorage2D(instance->m_ID, 1u, GetFormat(instance->m_Format), instance->m_Width, instance->m_Height);
			glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		});
	}

	void OpenGLRenderTarget::BindTexture(uint32_t slot) const noexcept
	{
		Ref<const OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->m_ID);
		});
	}

	void OpenGLRenderTarget::ReadPixel(void* data, uint32_t xCoord, uint32_t yCoord)
	{
		Ref<OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, data, xCoord, yCoord]()
		{
			GLenum format = 0, type = 0;
			switch (instance->m_Format)
			{
			case RenderTargetFormat::R8:
			case RenderTargetFormat::R32F:
				format = GL_RED;
				break;
			case RenderTargetFormat::R32_UINT:
				format = GL_RED_INTEGER;
				break;
			case RenderTargetFormat::RG8:
			case RenderTargetFormat::RG16F:
			case RenderTargetFormat::RG32F:
				format =  GL_RG;
				break;
			case RenderTargetFormat::RG32_UINT:
				format = GL_RG_INTEGER;
				break;
			case RenderTargetFormat::RGB8:
			case RenderTargetFormat::RGB32F:
				format = GL_RGB;
				break;
			case RenderTargetFormat::RGB32_UINT:
				format = GL_RGB_INTEGER;
				break;
			case RenderTargetFormat::RGBA8:
			case RenderTargetFormat::RGBA16F:
			case RenderTargetFormat::RGBA32F:
				format =  GL_RGBA;
				break;
			case RenderTargetFormat::RGBA32_UINT:
				format = GL_RGBA_INTEGER;
				break;
			}
			switch (instance->m_Format)
			{
			case RenderTargetFormat::R8:
			case RenderTargetFormat::RG8:
			case RenderTargetFormat::RGB8:
			case RenderTargetFormat::RGBA8:
				type = GL_UNSIGNED_BYTE;
				break;
			case RenderTargetFormat::R32_UINT:
			case RenderTargetFormat::RG32_UINT:
			case RenderTargetFormat::RGB32_UINT:
			case RenderTargetFormat::RGBA32_UINT:
				type = GL_UNSIGNED_INT;
				break;
			case RenderTargetFormat::R32F:
			case RenderTargetFormat::RG16F:
			case RenderTargetFormat::RG32F:
			case RenderTargetFormat::RGB32F:
			case RenderTargetFormat::RGBA16F:
			case RenderTargetFormat::RGBA32F:
				type = GL_FLOAT;
				break;
			}
			glGetTextureSubImage(instance->m_ID, 0, xCoord, yCoord, 0, 1, 1, 1, format, type, sizeof(glm::vec4), data);
		});
	}
}
