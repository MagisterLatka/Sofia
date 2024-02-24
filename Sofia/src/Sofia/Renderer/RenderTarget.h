#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	enum class RenderTargetFormat
	{
		None = 0,
		R8, R32F,
		RG8, RG16F, RG32F,
		RGB8, RGB32F,
		RGBA8, RGBA16F, RGBA32F,
		Last = RGBA32F
	};
	class RenderTarget : public RefCounted
	{
	public:
		SOF_CORE virtual ~RenderTarget() = default;

		SOF_CORE virtual void Bind() const noexcept = 0; //TODO move to render pass
		SOF_CORE virtual void Clear(const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) noexcept = 0;
		SOF_CORE virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) = 0;

		SOF_CORE virtual void BindTexture(uint32_t slot = 0u) const noexcept = 0;
		SOF_CORE virtual void* GetRawTexturePointer() const noexcept = 0;

		SOF_CORE static Ref<RenderTarget> Create(uint32_t width, uint32_t height, RenderTargetFormat format = RenderTargetFormat::RGBA8);
	};
}
