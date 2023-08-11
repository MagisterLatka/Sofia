#pragma once

#include "Sofia/Renderer/GraphicsBuffers.h"
#include "Sofia/Renderer/Shader.h"

namespace Sofia {

	class InputLayout : public RefCounted
	{
	public:
		virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index = 0) const noexcept = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const noexcept = 0;

		virtual void Bind() const noexcept = 0;

		static Ref<InputLayout> Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader = {}, const Ref<IndexBuffer>& indexBuffer = {});
	};
}
