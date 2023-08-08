#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	struct Buffer
	{
		enum class Allocator
		{
			None = 0, New, Malloc
		};
	private:
		bool m_Delete = false;
		Allocator m_Allocator = Allocator::None;
	public:
		void* Data = nullptr;
		uint32_t Size = 0;

		Buffer() = default;
		Buffer(void* data, uint32_t size, bool takeControl = false, Allocator allocator = Allocator::New) noexcept
			: Data(data), Size(size), m_Delete(takeControl), m_Allocator(allocator)
		{
			SOF_CORE_ASSERT(!takeControl || allocator != Allocator::None, "Sofia::Buffer expect to specify allocator");
		}
		Buffer(const Buffer& other) noexcept
			: Size(other.Size), m_Delete(true), m_Allocator(other.m_Allocator)
		{
			if (m_Allocator == Allocator::New)
				Data = new uint8_t[Size];
			else if (m_Allocator == Allocator::Malloc)
				Data = (uint8_t*)malloc(Size);
			memcpy(Data, other.Data, Size);
		}
		Buffer(Buffer&& other) noexcept
			: Size(other.Size), m_Delete(true), m_Allocator(other.m_Allocator)
		{
			Data = other.Data;
			other.Data = nullptr;
			other.Size = 0;
			other.m_Allocator = Allocator::None;
		}
		~Buffer()
		{
			if (m_Delete)
			{
				switch (m_Allocator)
				{
					case Allocator::New:
						if (Size > 1)
							delete Data;
						else delete[] Data;
						break;
					case Allocator::Malloc:
						free(Data);
						break;
				}
			}
		}

		Buffer& operator=(const Buffer& other)
		{
			if (m_Delete && Data)
			{
				switch (m_Allocator)
				{
					case Allocator::None:
					default:
						SOF_CORE_THROW_INFO("Invalid allocator");
					case Allocator::New:
						if (Size > 1)
							delete Data;
						else delete[] Data;
						break;
					case Allocator::Malloc:
						free(Data);
						break;
				}
			}
			m_Delete = true;
			m_Allocator = Allocator::New;

			Size = other.Size;
			Data = new uint8_t[Size];
			memcpy(Data, other.Data, Size);
			return *this;
		}
		Buffer& operator=(Buffer&& other)
		{
			if (m_Delete && Data)
			{
				switch (m_Allocator)
				{
					case Allocator::None:
					default:
						SOF_CORE_THROW_INFO("Invalid allocator");
					case Allocator::New:
						if (Size > 1)
							delete Data;
						else delete[] Data;
						break;
					case Allocator::Malloc:
						free(Data);
						break;
				}
			}

			m_Delete = other.m_Delete;
			m_Allocator = other.m_Allocator;
			Size = other.Size;
			Data = other.Data;
			other.Data = nullptr;
			other.Size = 0;
			other.m_Allocator = Allocator::None;
			return *this;
		}

		static Buffer Copy(void* data, uint32_t size) noexcept
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			if (size > Size)
			{
				if (m_Delete && Data)
				{
					switch (m_Allocator)
					{
						case Allocator::None:
						default:
							SOF_CORE_THROW_INFO("Invalid allocator");
							delete[] Data;
						case Allocator::New:
							if (Size > 1)
								delete Data;
							else delete[] Data;
							break;
						case Allocator::Malloc:
							free(Data);
							break;
					}
				}

				m_Allocator = Allocator::New;
				m_Delete = true;
				Data = new uint8_t[size];
				Size = size;
			}
		}

		void Zero() noexcept
		{
			if (Data) memset(Data, 0, Size);
		}

		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			SOF_CORE_ASSERT(offset < Size, "Accesing data out of buffer");
			return *(T*)(Data + offset);
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0)
		{
			SOF_CORE_ASSERT(Data, "Buffer has no memory allocated");
			SOF_CORE_ASSERT(size + offset < Size, "Accesing data out of buffer");
			memcpy((uint8_t*)Data + offset, data, size);
		}

		operator bool() noexcept { return Data; }
		template<typename T>
		T& operator[](uint32_t index)		{ SOF_CORE_ASSERT(index * sizeof(T) < Size); return ((T*)Data)[index]; }
		template<typename T>
		T operator[](uint32_t index) const	{ SOF_CORE_ASSERT(index * sizeof(T) < Size); return ((T*)Data)[index]; }

		template<typename T>
		T* As() noexcept { return (T*)Data; }

		uint32_t GetSize() const noexcept { return Size; }
	};
}
