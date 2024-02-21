#pragma once

#include "Sofia/Core.h"
#include "Sofia/Buffer.h"

namespace Sofia {

	enum struct BufferLayoutElementDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4
	};
	inline uint32_t BufferLayoutElementDataTypeSize(BufferLayoutElementDataType type)
	{
		switch (type)
		{
		case BufferLayoutElementDataType::Float:	return 1 * sizeof(float);
		case BufferLayoutElementDataType::Float2:	return 2 * sizeof(float);
		case BufferLayoutElementDataType::Float3:	return 3 * sizeof(float);
		case BufferLayoutElementDataType::Float4:	return 4 * sizeof(float);
		case BufferLayoutElementDataType::Mat3:		return 9 * sizeof(float);
		case BufferLayoutElementDataType::Mat4:		return 16 * sizeof(float);
		case BufferLayoutElementDataType::Int:		return 1 * sizeof(int);
		case BufferLayoutElementDataType::Int2:		return 2 * sizeof(int);
		case BufferLayoutElementDataType::Int3:		return 3 * sizeof(int);
		case BufferLayoutElementDataType::Int4:		return 4 * sizeof(int);
		case BufferLayoutElementDataType::UInt:		return 1 * sizeof(uint32_t);
		case BufferLayoutElementDataType::UInt2:	return 2 * sizeof(uint32_t);
		case BufferLayoutElementDataType::UInt3:	return 3 * sizeof(uint32_t);
		case BufferLayoutElementDataType::UInt4:	return 4 * sizeof(uint32_t);
		}

		SOF_CORE_THROW_INFO("Unknown buffer layout element data type");
		return 0;
	}
	struct BufferLayoutElement
	{
		std::string Name;
		BufferLayoutElementDataType Type = BufferLayoutElementDataType::None;
		uint32_t Size = 0u;
		uint32_t Offset = 0u;
		bool Normalized = false;
		uint32_t InstanceStepRate = 0u;

		BufferLayoutElement() = default;
		BufferLayoutElement(const std::string& name, BufferLayoutElementDataType type, bool normalized = false, uint32_t instanceStepRate = 0u)
			: Name(name), Type(type), Size(BufferLayoutElementDataTypeSize(type)), Normalized(normalized), InstanceStepRate(instanceStepRate) {}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case BufferLayoutElementDataType::Float:	return 1;
			case BufferLayoutElementDataType::Float2:	return 2;
			case BufferLayoutElementDataType::Float3:	return 3;
			case BufferLayoutElementDataType::Float4:	return 4;
			case BufferLayoutElementDataType::Mat3:		return 9;
			case BufferLayoutElementDataType::Mat4:		return 16;
			case BufferLayoutElementDataType::Int:		return 1;
			case BufferLayoutElementDataType::Int2:		return 2;
			case BufferLayoutElementDataType::Int3:		return 3;
			case BufferLayoutElementDataType::Int4:		return 4;
			case BufferLayoutElementDataType::UInt:		return 1;
			case BufferLayoutElementDataType::UInt2:	return 2;
			case BufferLayoutElementDataType::UInt3:	return 3;
			case BufferLayoutElementDataType::UInt4:	return 4;
			}

			SOF_CORE_THROW_INFO("Unknown buffer layout element data type");
			return 0;
		}
	};
	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
			: m_Elements(elements)
		{
			Calculate();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferLayoutElement>& GetElements() const { return m_Elements; }

		void AddElement(const BufferLayoutElement& element)
		{
			m_Elements.push_back(element);
			auto& e = m_Elements.back();
			e.Offset = m_Stride;
			m_Stride += e.Size;
		}

		std::vector<BufferLayoutElement>::iterator			begin() { return m_Elements.begin(); }
		std::vector<BufferLayoutElement>::iterator			end() { return m_Elements.end(); }
		std::vector<BufferLayoutElement>::const_iterator	begin() const { return m_Elements.begin(); }
		std::vector<BufferLayoutElement>::const_iterator	end() const { return m_Elements.end(); }
	private:
		void Calculate()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferLayoutElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	enum struct BufferUsage
	{
		None = 0, Default, Dynamic, Immutable
	};
	class VertexBuffer : public RefCounted
	{
	public:
		SOF_CORE virtual ~VertexBuffer() = default;

		SOF_CORE virtual void SetData(void* data, uint32_t size, uint32_t offset = 0u) = 0;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) = 0;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) = 0;

		SOF_CORE virtual const BufferLayout& GetLayout() const noexcept = 0;

		SOF_CORE virtual uint32_t GetSize() const noexcept = 0;

		SOF_CORE static Ref<VertexBuffer> Create(const BufferLayout& layout, void* data, uint32_t size, BufferUsage usage = BufferUsage::Default);
		SOF_CORE static Ref<VertexBuffer> Create(const BufferLayout& layout, const Buffer& buffer, BufferUsage usage = BufferUsage::Default);
		SOF_CORE static Ref<VertexBuffer> Create(const BufferLayout& layout, Buffer&& buffer, BufferUsage usage = BufferUsage::Default);
	};
	class IndexBuffer : public RefCounted
	{
	public:
		SOF_CORE virtual ~IndexBuffer() = default;

		SOF_CORE virtual void SetData(uint32_t* data, uint32_t size, uint32_t offset = 0u) = 0;
		SOF_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) = 0;
		SOF_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) = 0;

		SOF_CORE virtual uint32_t GetSize() const noexcept = 0;
		SOF_CORE virtual uint32_t GetIndicesCount() const noexcept = 0;

		SOF_CORE static Ref<IndexBuffer> Create(uint32_t* data, uint32_t size, BufferUsage usage = BufferUsage::Default);
		SOF_CORE static Ref<IndexBuffer> Create(const Buffer& buffer, BufferUsage usage = BufferUsage::Default);
		SOF_CORE static Ref<IndexBuffer> Create(Buffer&& buffer, BufferUsage usage = BufferUsage::Default);
	};

	enum class UniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3x3, Mat4x4
	};
	struct UniformDecl
	{
		UniformType Type;
		uint32_t Offset;
		std::string Name;
	};
	struct UniformBufferBase
	{
		virtual const uint8_t* GetBuffer() const noexcept = 0;
		virtual uint32_t GetBufferSize() const noexcept = 0;
		virtual const UniformDecl* GetUniforms() const noexcept = 0;
		virtual uint32_t GetUniformCount() const noexcept = 0;
	};
	template<uint32_t N, uint32_t U>
	struct UniformBuffer : public UniformBufferBase
	{
		static_assert(N % 16 == 0, "Uniform buffer size must be a multiple of 16 bytes");
		uint8_t Buffer[N];
		UniformDecl Uniforms[U];
		uint32_t Offset = 0;
		uint32_t index = 0;

		virtual const uint8_t* GetBuffer() const override { return Buffer; }
		virtual uint32_t GetBufferSize() const noexcept override { return N; };
		virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
		virtual uint32_t GetUniformCount() const override { return U; }

		template<typename T>
		void Push(const std::string& name, const T& data) noexcept {}
		template<>
		void Push(const std::string& name, const float& data)
		{
			SOF_CORE_ASSERT(index < U && Offset + sizeof(float) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Float, Offset, name };
			memcpy(Buffer + Offset, &data, sizeof(float));
			Offset += sizeof(float);
		}
		void Push(const std::string& name, const glm::vec2& data)
		{
			if (Offset % 16 > 8) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::vec2) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Float2, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec2));
			Offset += sizeof(glm::vec2);
		}
		void Push(const std::string& name, const glm::vec3& data)
		{
			if (Offset % 16 > 4) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::vec3) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Float3, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec3));
			Offset += sizeof(glm::vec3);
		}
		void Push(const std::string& name, const glm::vec4& data)
		{
			if (Offset % 16 != 0) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::vec4) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Float4, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec4));
			Offset += sizeof(glm::vec4);
		}
		void Push(const std::string& name, const int& data)
		{
			SOF_CORE_ASSERT(index < U && Offset + sizeof(int) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Int, Offset, name };
			memcpy(Buffer + Offset, &data, sizeof(int));
			Offset += sizeof(int);
		}
		void Push(const std::string& name, const glm::ivec2& data)
		{
			if (Offset % 16 > 8) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::ivec2) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Int2, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec2));
			Offset += sizeof(glm::ivec2);
		}
		void Push(const std::string& name, const glm::ivec3& data)
		{
			if (Offset % 16 > 4) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::ivec3) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Int3, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec3));
			Offset += sizeof(glm::ivec3);
		}
		void Push(const std::string& name, const glm::ivec4& data)
		{
			if (Offset % 16 != 0) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::ivec4) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Int4, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec4));
			Offset += sizeof(glm::ivec4);
		}
		void Push(const std::string& name, const glm::mat3& data)
		{
			if (Offset % 16 != 0) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::mat3) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Mat3x3, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::mat3));
			Offset += sizeof(glm::mat3);
		}
		void Push(const std::string& name, const glm::mat4& data)
		{
			if (Offset % 16 != 0) Offset = (Offset / 16 + 1) * 16;
			SOF_CORE_ASSERT(index < U && Offset + sizeof(glm::mat4) <= N, "Uniform buffer overflow");
			Uniforms[index++] = { UniformType::Mat4x4, Offset, name };
			memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::mat4));
			Offset += sizeof(glm::mat4);
		}
	};
	enum class BufferShaderBinding
	{
		None = 0, Vertex, Fragment
	};
	class ConstantBuffer : public RefCounted
	{
	public:
		SOF_CORE virtual ~ConstantBuffer() = default;

		SOF_CORE virtual void SetData(uint32_t* data, uint32_t size) = 0;
		SOF_CORE virtual void SetData(const Buffer& buffer) = 0;
		SOF_CORE virtual void SetData(Buffer&& buffer) = 0;
		SOF_CORE virtual void SetData(const UniformBufferBase& buffer) = 0;

		SOF_CORE virtual void Bind(uint32_t slot = 0) const = 0;

		SOF_CORE virtual uint32_t GetSize() const noexcept = 0;

		SOF_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, void* data, uint32_t size);
		SOF_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, const Buffer& buffer);
		SOF_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, Buffer&& buffer);
		SOF_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, const UniformBufferBase& buffer);
	};
}
