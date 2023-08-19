#pragma once

#include "Sofia/Core.h"
#include "Sofia/Buffer.h"

namespace Sofia {

	enum class TextureFormat
	{
		None = 0,
		R8, R32F,
		RGB8, RGB32F,
		RGBA8, RGBA32F
	};
	struct Texture2DProps
	{
		uint32_t Width = 0, Height = 0;
		std::filesystem::path Filepath;
		TextureFormat Format;
		bool GenerateMipMaps = false;
		bool UseSRGB = false;
	};
	class Texture2D : public RefCounted
	{
	public:
		virtual ~Texture2D() = default;

		virtual void Bind(uint32_t slot) const noexcept = 0;

		virtual const Texture2DProps& GetProps() const noexcept = 0;
		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;
		virtual void* GetRawPointer() const noexcept = 0;

		static Ref<Texture2D> Create(const Texture2DProps& props);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);

		static uint32_t GetBPP(TextureFormat format);
		static uint32_t CalcNrMips(uint32_t width, uint32_t height);
		static void* Decode(void* const data, uint32_t size, uint32_t& width, uint32_t& height);
	};
}
