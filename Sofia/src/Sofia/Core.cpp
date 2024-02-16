#include <pch.h>
#include "Core.h"

#include "Sofia/Log.h"

#define SOF_BUILD_ID "v0.1a"

namespace Sofia {

	void InitializeCore()
	{
		Log::Init();

		SOF_CORE_TRACE("Sofia Engine {}", SOF_BUILD_ID);
		SOF_CORE_TRACE("Initializing...");
	}
	void ShutdownCore()
	{
		SOF_CORE_TRACE("Shutting down...");
	}
	uint64_t GetTime() noexcept
	{
		return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
}
