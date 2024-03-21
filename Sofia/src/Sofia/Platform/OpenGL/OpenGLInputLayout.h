#pragma once

#include "Sofia/Renderer/InputLayout.h"
#include "Sofia/Platform/OpenGL/OpenGLGraphicsBuffers.h"

namespace Sofia {

	class OpenGLInputLayout : public InputLayout
	{
	public:
		SOF_CORE OpenGLInputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer);
		SOF_CORE ~OpenGLInputLayout();

		SOF_CORE virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index) const noexcept override { return m_VertexBuffers[index]; }
		SOF_CORE virtual Ref<IndexBuffer> GetIndexBuffer() const noexcept override { return m_IndexBuffer; }

		SOF_CORE virtual void Bind() const noexcept override;
	private:
		void Create();
	private:
		uint32_t m_ID;
		std::vector<Ref<OpenGLVertexBuffer>> m_VertexBuffers;
		Ref<OpenGLIndexBuffer> m_IndexBuffer;
	};
}
