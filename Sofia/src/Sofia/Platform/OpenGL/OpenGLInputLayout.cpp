#include <pch.h>
#include "OpenGLInputLayout.h"

#include "Sofia/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Sofia {

	OpenGLInputLayout::OpenGLInputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer)
		: m_IndexBuffer(indexBuffer)
	{
		SOF_CORE_ASSERT(vertexBuffers.size() > 0, "Input layout requires at least vertex buffer");
		for (auto buffer = vertexBuffers.begin(); buffer != vertexBuffers.end(); ++buffer)
			m_VertexBuffers.push_back(*buffer);
		Create();
	}
	OpenGLInputLayout::~OpenGLInputLayout()
	{
		glDeleteVertexArrays(1, &m_ID);
	}
	void OpenGLInputLayout::Create()
	{
		Ref<OpenGLInputLayout> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateVertexArrays(1, &instance->m_ID);
			glBindVertexArray(instance->m_ID);
			for (auto& buffer : instance->m_VertexBuffers)
				buffer->Bind();
			if (instance->m_IndexBuffer)
				instance->m_IndexBuffer->Bind();

			uint32_t index = 0u;
			for (auto& buffer : instance->m_VertexBuffers)
			{
				for (auto element : buffer->GetLayout())
				{
					switch (element.Type)
					{
					case BufferLayoutElementDataType::Mat3:
						for (int i = 0; i < 3; ++i)
						{
							glEnableVertexAttribArray(index);
							glVertexAttribPointer(index++, 3, GL_FLOAT, element.Normalized, buffer->GetLayout().GetStride(),
								(const void*)(uint64_t)(element.Offset + sizeof(float) * 3 * i));
						}
						break;
					case BufferLayoutElementDataType::Mat4:
						for (int i = 0; i < 4; ++i)
						{
							glEnableVertexAttribArray(index);
							glVertexAttribPointer(index++, 4, GL_FLOAT, element.Normalized, buffer->GetLayout().GetStride(),
								(const void*)(uint64_t)(element.Offset + sizeof(float) * 4 * i));
						}
						break;
					case BufferLayoutElementDataType::Float:
					case BufferLayoutElementDataType::Float2:
					case BufferLayoutElementDataType::Float3:
					case BufferLayoutElementDataType::Float4:
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index++, element.GetComponentCount(), GL_FLOAT, element.Normalized, buffer->GetLayout().GetStride(), (const void*)(uint64_t)(element.Offset));
						break;
					case BufferLayoutElementDataType::Int:
					case BufferLayoutElementDataType::Int2:
					case BufferLayoutElementDataType::Int3:
					case BufferLayoutElementDataType::Int4:
						glEnableVertexAttribArray(index);
						glVertexAttribIPointer(index++, element.GetComponentCount(), GL_INT, buffer->GetLayout().GetStride(), (const void*)(uint64_t)(element.Offset));
						break;
					case BufferLayoutElementDataType::UInt:
					case BufferLayoutElementDataType::UInt2:
					case BufferLayoutElementDataType::UInt3:
					case BufferLayoutElementDataType::UInt4:
						glEnableVertexAttribArray(index);
						glVertexAttribIPointer(index++, element.GetComponentCount(), GL_UNSIGNED_INT, buffer->GetLayout().GetStride(), (const void*)(uint64_t)(element.Offset));
						break;
					}
				}
			}
		});
	}

	void OpenGLInputLayout::Bind() const noexcept
	{
		Ref<const OpenGLInputLayout> instance = this;
		Renderer::Submit([instance]()
		{
			glBindVertexArray(instance->m_ID);
			for (auto& buffer : instance->m_VertexBuffers)
				buffer->Bind();
			if (instance->m_IndexBuffer)
				instance->m_IndexBuffer->Bind();
		});
	}
}
