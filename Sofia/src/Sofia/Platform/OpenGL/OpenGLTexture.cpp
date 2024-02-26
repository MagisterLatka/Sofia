#include <pch.h>
#include "OpenGLTexture.h"

#include "Sofia/Renderer/Renderer.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Sofia {

	static GLenum OpenGLTextureInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:			return GL_R8;
		case TextureFormat::R32F:		return GL_R32F;
		case TextureFormat::RGB8:		return GL_RGB8;
		case TextureFormat::RGB32F:		return GL_RGB32F;
		case TextureFormat::RGBA8:		return GL_RGBA8;
		case TextureFormat::RGBA_SRGB:	return GL_SRGB8;
		case TextureFormat::RGBA32F:	return GL_RGBA32F;
		}
		SOF_CORE_THROW_INFO("Unknown texture format");
		return 0;
	}
	static GLenum OpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:
		case TextureFormat::R32F:
			return GL_RED;
		case TextureFormat::RGB8:
		case TextureFormat::RGB32F:
			return GL_RGB;
		case TextureFormat::RGBA8:
		case TextureFormat::RGBA_SRGB:
		case TextureFormat::RGBA32F:
			return GL_RGBA;
		}
		SOF_CORE_THROW_INFO("Unknown texture format");
		return 0;
	}
	static GLenum OpenGLTextureDataType(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:
		case TextureFormat::RGB8:
		case TextureFormat::RGBA8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::R32F:
		case TextureFormat::RGB32F:
		case TextureFormat::RGBA32F:
			return GL_FLOAT;
		}
		SOF_CORE_THROW_INFO("Unknown texture format");
		return 0;
	}
	static GLenum OpenGLTextureGetWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
		case Sofia::TextureWrap::Repeat:	return GL_REPEAT;
		case Sofia::TextureWrap::Clamp:		return GL_CLAMP_TO_EDGE;
		case Sofia::TextureWrap::Mirror:	return GL_MIRRORED_REPEAT;
		case Sofia::TextureWrap::Border:	return GL_CLAMP_TO_BORDER;
		}
		SOF_CORE_THROW_INFO("Unknown texture wrap");
		return 0;
	}
	OpenGLTexture2D::OpenGLTexture2D(const Texture2DProps& props)
		: m_Props(props)
	{
		if (props.Filepath.empty())
			SOF_CORE_ASSERT(props.Width > 0u && props.Height > 0u, "Texture expects non-zero width and height");
		SOF_CORE_ASSERT((uint32_t)props.Format > 0 && (uint32_t)props.Format <= (uint32_t)TextureFormat::Last, "Unknown texture format");
		Init();
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data, TextureFormat format)
	{
		SOF_CORE_ASSERT(width > 0u && height > 0u, "Texture expects non-zero width and height");
		SOF_CORE_ASSERT((uint32_t)format > 0 && (uint32_t)format <= (uint32_t)TextureFormat::Last, "Unknown texture format");
		m_Props.Width = width;
		m_Props.Height = height;
		m_Props.Format = format;
		m_Buffer = Buffer::Copy(data, width * height * GetBPP(format), Buffer::Allocator::Malloc);
		Init();
	}
	void OpenGLTexture2D::Init()
	{
		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			if (!instance->m_Props.Filepath.empty())
			{
				std::string filepath = instance->m_Props.Filepath.string();
				int width = 0, height = 0, channels;
				switch (instance->m_Props.Format)
				{
				case Sofia::TextureFormat::R8:
					instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 1); break;
				case Sofia::TextureFormat::R32F:
					instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 1); break;
				case Sofia::TextureFormat::RGB8:
					instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 4); break;
				case Sofia::TextureFormat::RGB32F:
					instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 3); break;
				case Sofia::TextureFormat::RGBA8:
					instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 4); break;
				case Sofia::TextureFormat::RGBA32F:
					instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 4); break;
				}
				SOF_CORE_ASSERT(instance->m_Buffer.Data, "Failed to load texture from file {0}", filepath);
				instance->m_Props.Width = width;
				instance->m_Props.Height = height;
			}

			uint32_t levels = CalcNrMips(instance->m_Props.Width, instance->m_Props.Height);
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->m_ID);
			glTextureStorage2D(instance->m_ID, instance->m_Props.GenerateMipMaps ? levels : 1,
				OpenGLTextureInternalFormat(instance->m_Props.Format), instance->m_Props.Width, instance->m_Props.Height);
			glTextureSubImage2D(instance->m_ID, 0, 0, 0, instance->m_Props.Width, instance->m_Props.Height, OpenGLTextureFormat(instance->m_Props.Format),
				OpenGLTextureDataType(instance->m_Props.Format), instance->m_Buffer.Data);

			glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_S, OpenGLTextureGetWrap(instance->m_Props.Wrap));
			glTextureParameteri(instance->m_ID, GL_TEXTURE_WRAP_T, OpenGLTextureGetWrap(instance->m_Props.Wrap));
			switch (instance->m_Props.Sampling)
			{
			case TextureSampling::Point:
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
				break;
			case TextureSampling::MinPointMagLinear:
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
				break;
			case TextureSampling::MinLinearMagPoint:
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
				break;
			case TextureSampling::Linear:
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
				break;
			case TextureSampling::Anisotropic:
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(instance->m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
				float maxi = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxi);
				glTextureParameterf(instance->m_ID, GL_TEXTURE_MAX_ANISOTROPY, glm::clamp((float)instance->m_Props.MaxAnisotropy, 1.0f, maxi));
				break;
			}

			glGenerateTextureMipmap(instance->m_ID);

		});
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const noexcept
	{
		Ref<const OpenGLTexture2D> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->m_ID);
		});
	}
}
