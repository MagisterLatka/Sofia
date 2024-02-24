#include <pch.h>
#include "OpenGLRenderTarget.h"

#include "Sofia/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Sofia {

	static GLenum GetFormat(RenderTargetFormat format)
	{
		switch (format)
		{
		case Sofia::RenderTargetFormat::R8:			return GL_R8;
		case Sofia::RenderTargetFormat::R32F:		return GL_R32F;
		case Sofia::RenderTargetFormat::RG8:		return GL_RG8;
		case Sofia::RenderTargetFormat::RG16F:		return GL_RG16F;
		case Sofia::RenderTargetFormat::RG32F:		return GL_RG32F;
		case Sofia::RenderTargetFormat::RGB8:		return GL_RGB8;
		case Sofia::RenderTargetFormat::RGB32F:		return GL_RGB32F;
		case Sofia::RenderTargetFormat::RGBA8:		return GL_RGBA8;
		case Sofia::RenderTargetFormat::RGBA16F:	return GL_RGBA16F;
		case Sofia::RenderTargetFormat::RGBA32F:	return GL_RGBA32F;
		}
		SOF_CORE_THROW_INFO("Unknown render target format");
		return 0;
	}
	OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format)
		: m_Width(width), m_Height(height), m_Format(format)
	{
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
		SOF_CORE_ASSERT((uint32_t)format > 0 && (uint32_t)format <= (uint32_t)RenderTargetFormat::Last, "Unknown render target format");
		Resize(m_Width, m_Height, true);
	}
	OpenGLRenderTarget::~OpenGLRenderTarget()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_Texture);
	}
	void OpenGLRenderTarget::Resize(uint32_t width, uint32_t height, bool forceResize)
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
				glDeleteFramebuffers(1, &instance->m_ID);
				glDeleteTextures(1, &instance->m_Texture);
			}

			glCreateFramebuffers(1, &instance->m_ID);
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->m_Texture);
			glTextureStorage2D(instance->m_Texture, 1u, GetFormat(instance->m_Format), instance->m_Width, instance->m_Height);
			glTextureParameteri(instance->m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(instance->m_Texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(instance->m_Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(instance->m_Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glNamedFramebufferTexture(instance->m_ID, GL_COLOR_ATTACHMENT0, instance->m_Texture, 0);
		});
	}

	void OpenGLRenderTarget::Bind() const noexcept
	{
		Ref<const OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance]()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, instance->m_ID);
		});
	}

	void OpenGLRenderTarget::Clear(const glm::vec4& clearVal) noexcept
	{
		Ref<OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, clearVal]()
		{
			glClearNamedFramebufferfv(instance->m_ID, GL_COLOR, 0, glm::value_ptr(clearVal));
		});
	}

	void OpenGLRenderTarget::BindTexture(uint32_t slot) const noexcept
	{
		Ref<const OpenGLRenderTarget> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->m_Texture);
		});
	}
}
