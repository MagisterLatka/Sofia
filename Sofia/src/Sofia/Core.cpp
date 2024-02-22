#include <pch.h>
#include "Core.h"

#include "Sofia/Log.h"

#define SOF_BUILD_ID "v0.1a"

namespace Sofia {

	static std::filesystem::path s_Current;

	void InitializeCore(int argc, char** argv, char** envp)
	{
#if defined(SOF_PLATFORM_LINUX)
		s_Current = std::filesystem::current_path();
		std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
#endif
		Log::Init();
		SOF_CORE_TRACE("Sofia Engine {}", SOF_BUILD_ID);
		SOF_CORE_TRACE("Initializing...");
	}
	void ShutdownCore()
	{
		SOF_CORE_TRACE("Shutting down...");
#if defined(SOF_PLATFORM_LINUX)
		std::filesystem::current_path(s_Current);
#endif
	}
	uint64_t GetTime() noexcept
	{
		return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
}
