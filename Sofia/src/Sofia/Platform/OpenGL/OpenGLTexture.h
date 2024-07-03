#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		SOF_CORE OpenGLTexture2D(const Texture2DProps& props);
		SOF_CORE OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
		SOF_CORE virtual ~OpenGLTexture2D();

		SOF_CORE virtual void Bind(uint32_t slot) const noexcept override;

		SOF_CORE virtual const Texture2DProps& GetProps() const noexcept override { return m_Props; }
		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Props.Width; }
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Props.Height; }
		SOF_CORE virtual void* GetRawPointer() const noexcept override { return (void*)(uint64_t)m_ID; }

		SOF_CORE virtual bool IsLoaded() const noexcept override { return m_Loaded; }
	private:
		void Init();
	private:
		uint32_t m_ID;
		Texture2DProps m_Props;
		bool m_Loaded;

		Sofia::Buffer m_Buffer;
	};
}
