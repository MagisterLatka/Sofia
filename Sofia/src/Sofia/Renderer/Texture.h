#pragma once

#include "Sofia/Core.h"
#include "Sofia/Buffer.h"

namespace Sofia {

	enum class TextureFormat
	{
		None = 0,
		R8, R32F,
		RGB8, RGB32F,
		RGBA8, RGBA_SRGB, RGBA32F,
		Last = RGBA32F
	};
	enum class TextureSampling
	{
		None = 0, Point, MinPointMagLinear, MinLinearMagPoint, Linear, Anisotropic
	};
	enum class TextureWrap
	{
		None = 0, Repeat, Clamp, Mirror, Border
	};
	struct Texture2DProps
	{
		uint32_t Width = 0u, Height = 0u;
		std::filesystem::path Filepath;
		TextureFormat Format = TextureFormat::RGBA8;
		bool GenerateMipMaps = false;
		TextureSampling Sampling = TextureSampling::MinPointMagLinear;
		uint32_t MaxAnisotropy = 1u; //for TextureSampling::Anisotropic
		TextureWrap Wrap = TextureWrap::Clamp;
		glm::vec4 BorderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //for TextureWrap::Border
	};
	class Texture2D : public RefCounted
	{
	public:
		SOF_CORE virtual ~Texture2D() = default;

		SOF_CORE virtual void Bind(uint32_t slot) const noexcept = 0;

		SOF_CORE virtual const Texture2DProps& GetProps() const noexcept = 0;
		SOF_CORE virtual uint32_t GetWidth() const noexcept = 0;
		SOF_CORE virtual uint32_t GetHeight() const noexcept = 0;
		SOF_CORE virtual void* GetRawPointer() const noexcept = 0;

		SOF_CORE static Ref<Texture2D> Create(const Texture2DProps& props);
		SOF_CORE static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);

		SOF_CORE static uint32_t GetBPP(TextureFormat format);
		SOF_CORE static uint32_t CalcNrMips(uint32_t width, uint32_t height);
		SOF_CORE static void* Decode(void* const data, uint32_t size, uint32_t& width, uint32_t& height);
	};
}
