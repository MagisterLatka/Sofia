#pragma once

#include "Sofia/Renderer/GraphicsBuffers.h"
#include <d3d11.h>

namespace Sofia {

	class DX11VertexBuffer : public VertexBuffer
	{
		friend class DX11InputLayout;
	public:
		SOF_CORE DX11VertexBuffer(const BufferLayout& layout, void* data, uint32_t size, BufferUsage usage);
		SOF_CORE DX11VertexBuffer(const BufferLayout& layout, const Buffer& buffer, BufferUsage usage);
		SOF_CORE DX11VertexBuffer(const BufferLayout& layout, Buffer&& buffer, BufferUsage usage);
		SOF_CORE virtual ~DX11VertexBuffer() = default;

		SOF_CORE virtual void SetData(void* data, uint32_t size, uint32_t offset = 0u) override;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) override;

		SOF_CORE virtual const BufferLayout& GetLayout() const noexcept override { return m_Layout; }

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update(uint32_t offset);
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
		SOF_CORE DX11IndexBuffer(uint32_t* data, uint32_t size, BufferUsage usage);
		SOF_CORE DX11IndexBuffer(const Buffer& buffer, BufferUsage usage);
		SOF_CORE DX11IndexBuffer(Buffer&& buffer, BufferUsage usage);
		SOF_CORE virtual ~DX11IndexBuffer() = default;

		SOF_CORE virtual void SetData(uint32_t* data, uint32_t size, uint32_t offset) override;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset) override;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset) override;

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
		SOF_CORE virtual uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update(uint32_t offset);
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size = 0u;
		Buffer m_Data;
		BufferUsage m_Usage;
	};

	class DX11ConstantBuffer : public ConstantBuffer
	{
	public:
		SOF_CORE DX11ConstantBuffer(BufferShaderBinding binding, void* data, uint32_t size);
		SOF_CORE DX11ConstantBuffer(BufferShaderBinding binding, const Buffer& buffer);
		SOF_CORE DX11ConstantBuffer(BufferShaderBinding binding, Buffer&& buffer);
		SOF_CORE DX11ConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer);
		SOF_CORE virtual ~DX11ConstantBuffer() = default;

		SOF_CORE virtual void SetData(uint32_t* data, uint32_t size) override;
		SOF_CORE virtual void SetData(const Buffer& buffer) override;
		SOF_CORE virtual void SetData(Buffer&& buffer) override;
		SOF_CORE virtual void SetData(const UniformBufferBase& buffer) override;

		SOF_CORE virtual void Bind(uint32_t slot) const override;

		SOF_CORE virtual uint32_t GetSize() const noexcept override { return m_Size; }
	private:
		SOF_CORE void Create();
		SOF_CORE void Update();
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size = 0u;
		Buffer m_Data;

		BufferShaderBinding m_Binding;
	};
}
