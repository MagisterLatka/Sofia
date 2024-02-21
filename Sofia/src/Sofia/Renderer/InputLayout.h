#pragma once

#include "Sofia/Core.h"
#include "Sofia/Renderer/GraphicsBuffers.h"
#include "Sofia/Renderer/Shader.h"

namespace Sofia {

	class InputLayout : public RefCounted
	{
	public:
		SOF_CORE virtual ~InputLayout() = default;

		SOF_CORE virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index = 0) const noexcept = 0;
		SOF_CORE virtual Ref<IndexBuffer> GetIndexBuffer() const noexcept = 0;

		SOF_CORE virtual void Bind() const noexcept = 0;

		SOF_CORE static Ref<InputLayout> Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader = {}, const Ref<IndexBuffer>& indexBuffer = {});
	};
}
