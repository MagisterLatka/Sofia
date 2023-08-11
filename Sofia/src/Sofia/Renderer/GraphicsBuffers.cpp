#include <pch.h>
#include "GraphicsBuffers.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Platform/DX11/DX11GraphicsBuffers.h"

namespace Sofia {

	Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, void* data, uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11VertexBuffer>::Create(layout, data, size, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, const Buffer& buffer, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11VertexBuffer>::Create(layout, buffer, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, Buffer&& buffer, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11VertexBuffer>::Create(layout, buffer, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}



	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11IndexBuffer>::Create(data, size, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(const Buffer& buffer, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11IndexBuffer>::Create(buffer, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(Buffer&& buffer, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11IndexBuffer>::Create(buffer, usage);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}



	Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, void* data, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11ConstantBuffer>::Create(binding, data, size);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, const Buffer& buffer)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11ConstantBuffer>::Create(binding, buffer);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, Buffer&& buffer)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11ConstantBuffer>::Create(binding, buffer);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, const UniformBufferBase& buffer)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11ConstantBuffer>::Create(binding, buffer);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
}
