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
			if ((uint32_t)instance->m_Format & 0x30)
			{
				if (instance->m_ID)
					glDeleteRenderbuffers(1, &instance->m_ID);

				glCreateRenderbuffers(1, &instance->m_ID);
				glNamedRenderbufferStorage(instance->m_ID, instance->m_Format == RenderTargetFormat::Depth24Stencil8 ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT32F,
					instance->m_Width, instance->m_Height);
			}
			else
			{
				if (instance->m_ID)
					glDeleteTextures(1, &instance->m_ID);

				glCreateTextures(GL_TEXTURE_2D, 1, &instance->m_ID);
				glTextureStorage2D(instance->m_ID, 1u, GetFormat(instance->m_Format), instance->m_Width, instance->m_Height);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glCreateTextures(GL_TEXTURE_2D, 1, &instance->m_ReadBuffer);
				glTextureStorage2D(instance->m_ReadBuffer, 1u, GetFormat(instance->m_Format), 1u, 1u);
			}
		});
	}

	void OpenGLRenderTarget::BindTexture(uint32_t slot) const
	{
		SOF_CORE_ASSERT(!((uint32_t)m_Format & 0x30u), "Cannot bind depth or depth stencil format");
		Ref<const OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->m_ID);
		});
	}

	void OpenGLRenderTarget::ReadPixel(void* data, uint32_t xCoord, uint32_t yCoord)
	{
		SOF_CORE_ASSERT(!((uint32_t)m_Format & 0x30u), "Cannot read from depth or depth stencil format");
		Ref<OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, data, xCoord, yCoord]()
		{
			glCopyImageSubData(instance->m_ID, GL_TEXTURE_2D, 0u, xCoord, yCoord, 0u, instance->m_ReadBuffer, GL_TEXTURE_2D, 0u, 0u, 0u, 0u, 1u, 1u, 1u);

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
			glGetTextureImage(instance->m_ReadBuffer, 0u, format, type, sizeof(glm::vec4), data);
		});
	}
}
