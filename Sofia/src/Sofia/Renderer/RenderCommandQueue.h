#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class RenderCommandQueue
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		SOF_CORE RenderCommandQueue() noexcept;
		SOF_CORE ~RenderCommandQueue();

		SOF_CORE void* Allocate(RenderCommandFn fn, uint32_t size) noexcept;

		SOF_CORE void Execute();
	private:
		uint8_t* m_CommandBuffer;
		uint8_t* m_CommandBufferPtr;
		uint32_t m_CommandCount = 0;
	};
}
