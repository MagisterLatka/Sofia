#pragma once

#if defined(SOF_BUILD_DLL)
#	if defined(SOF_PLATFORM_WINDOWS)
#		define SOF_CORE __declspec(dllexport)
#	else
#		define SOF_CORE
#	endif
#else
#	if defined(SOF_PLATFORM_WINDOWS)
#		define SOF_CORE __declspec(dllimport)
#	else
#		define SOF_CORE
#	endif
#endif

#include "Sofia/Exception.h"

#define SOF_EXPAND(x) x
#define SOF_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)

#define SOF_CORE_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__((message) += Sofia::Format(": {-1}", -1, __VA_ARGS__));\
									SOF_CORE_CRITICAL(message); throw Sofia::MessageException(__LINE__, __FILE__, (message), true); } }
#define SOF_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__((message) += Sofia::Format(": {-1}", -1, __VA_ARGS__));\
									SOF_CRITICAL(message); throw Sofia::MessageException(__LINE__, __FILE__, (message), false); } }

#define SOF_CORE_THROW throw Sofia::SofiaException(__LINE__, __FILE__, true);
#define SOF_THROW throw Sofia::SofiaException(__LINE__, __FILE__, false);

#define SOF_CORE_THROW_INFO(message) { SOF_CORE_CRITICAL(message);  throw Sofia::MessageException(__LINE__, __FILE__, (message), true); }
#define SOF_THROW_INFO(message) { SOF_CRITICAL(message);  throw Sofia::MessageException(__LINE__, __FILE__, (message), false); }

#include "Sofia/Timer.h"

namespace Sofia {

	void SOF_CORE InitializeCore(int argc, char** argv, char** envp);
	void SOF_CORE ShutdownCore();
	uint64_t SOF_CORE GetTime() noexcept;
}

#include "Sofia/Ref.h"

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ...Args>
Scope<T> CreateScope(Args&& ...args) noexcept
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}
