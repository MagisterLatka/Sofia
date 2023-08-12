#pragma once

#include "Sofia/Renderer/GraphicsBuffers.h"
#include <d3d11.h>

namespace Sofia {

	class DX11VertexBuffer : public VertexBuffer
	{
		friend class DX11InputLayout;
	public:
		DX11VertexBuffer(const BufferLayout& layout, void* data, uint32_t size, BufferUsage usage);
		DX11VertexBuffer(const BufferLayout& layout, const Buffer& buffer, BufferUsage usage);
		DX11VertexBuffer(const BufferLayout& layout, Buffer&& buffer, BufferUsage usage);
		virtual ~DX11VertexBuffer();

		virtual void SetData(void* data, uint32_t size, uint32_t offset = 0u) override;
		virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
		virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) override;

		virtual const BufferLayout& GetLayout() const noexcept override { return m_Layout; }

		virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		void Create();
		void Update(uint32_t offset);
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size = 0u;
		Buffer m_Data;
		BufferUsage m_Usage;

		BufferLayout m_Layout;
	};

	class DX11IndexBuffer : public IndexBuffer
	{
		friend class DX11InputLayout;
	public:
		DX11IndexBuffer(uint32_t* data, uint32_t size, BufferUsage usage);
		DX11IndexBuffer(const Buffer& buffer, BufferUsage usage);
		DX11IndexBuffer(Buffer&& buffer, BufferUsage usage);
		virtual ~DX11IndexBuffer();

		virtual void SetData(uint32_t* data, uint32_t size, uint32_t offset) override;
		virtual void SetData(const Buffer& buffer, uint32_t offset) override;
		virtual void SetData(Buffer&& buffer, uint32_t offset) override;

		virtual uint32_t GetSize() const noexcept override { return m_Size; }
		virtual uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
	private:
		void Create();
		void Update(uint32_t offset);
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size = 0u;
		Buffer m_Data;
		BufferUsage m_Usage;
	};

	class DX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DX11ConstantBuffer(BufferShaderBinding binding, void* data, uint32_t size);
		DX11ConstantBuffer(BufferShaderBinding binding, const Buffer& buffer);
		DX11ConstantBuffer(BufferShaderBinding binding, Buffer&& buffer);
		DX11ConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer);
		virtual ~DX11ConstantBuffer();

		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetData(const Buffer& buffer) override;
		virtual void SetData(Buffer&& buffer) override;
		virtual void SetData(const UniformBufferBase& buffer) override;

		virtual void Bind() const override;

		virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		void Create();
		void Update();
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size = 0u;
		Buffer m_Data;

		BufferShaderBinding m_Binding;
	};
}
