#pragma once

#include "Sofia/Renderer/GraphicsBuffers.h"

namespace Sofia {

	class OpenGLVertexBuffer : public VertexBuffer
	{
		friend class OpenGLInputLayout;
	public:
		SOF_CORE OpenGLVertexBuffer(const BufferLayout& layout, void* data, uint32_t size, BufferUsage usage);
		SOF_CORE OpenGLVertexBuffer(const BufferLayout& layout, const Buffer& buffer, BufferUsage usage);
		SOF_CORE OpenGLVertexBuffer(const BufferLayout& layout, Buffer&& buffer, BufferUsage usage);
		SOF_CORE OpenGLVertexBuffer(const BufferLayout& layout, void* data, uint32_t size);
		SOF_CORE virtual ~OpenGLVertexBuffer();

		SOF_CORE virtual void SetData(void* data, uint32_t size, uint32_t offset = 0u) override;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) override;
		SOF_CORE virtual void SetData() override;

		SOF_CORE virtual const BufferLayout& GetLayout() const noexcept override { return m_Layout; }

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update(uint32_t offset);
		SOF_CORE void Bind() const;
	private:
		uint32_t m_ID = 0u, m_Size = 0u;
		Buffer m_Data;
		BufferUsage m_Usage;

		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
		friend class OpenGLInputLayout;
	public:
		SOF_CORE OpenGLIndexBuffer(uint32_t* data, uint32_t size, BufferUsage usage);
		SOF_CORE OpenGLIndexBuffer(const Buffer& buffer, BufferUsage usage);
		SOF_CORE OpenGLIndexBuffer(Buffer&& buffer, BufferUsage usage);
		SOF_CORE virtual ~OpenGLIndexBuffer();

		SOF_CORE virtual void SetData(uint32_t* data, uint32_t size, uint32_t offset) override;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset) override;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset) override;

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
		SOF_CORE virtual uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update(uint32_t offset);
		SOF_CORE void Bind() const;
	private:
		uint32_t m_ID = 0u, m_Size = 0u;
		Buffer m_Data;
		BufferUsage m_Usage;
	};

	class OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		SOF_CORE OpenGLConstantBuffer(BufferShaderBinding binding, void* data, uint32_t size);
		SOF_CORE OpenGLConstantBuffer(BufferShaderBinding binding, const Buffer& buffer);
		SOF_CORE OpenGLConstantBuffer(BufferShaderBinding binding, Buffer&& buffer);
		SOF_CORE OpenGLConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer);
		SOF_CORE virtual ~OpenGLConstantBuffer();

		SOF_CORE virtual void SetData(void* data, uint32_t size) override;
		SOF_CORE virtual void SetData(const Buffer& buffer) override;
		SOF_CORE virtual void SetData(Buffer&& buffer) override;
		SOF_CORE virtual void SetData(const UniformBufferBase& buffer) override;

		SOF_CORE virtual void Bind(uint32_t slot) const override;

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update();
	private:
		uint32_t m_ID = 0u, m_Size = 0u;
		Buffer m_Data;
	};
}
