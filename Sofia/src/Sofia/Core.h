#pragma once

#define SOF_EXPAND(x) x

#include "Sofia/Exception.h"

#define SOF_CORE_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__(message += Sofia::Format(": {-1}", -1, __VA_ARGS__));\
									SOF_CORE_CRITICAL(message); throw Sofia::MessageException(__LINE__, __FILE__, message, true); } }
#define SOF_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__(message += Sofia::Format(": {-1}", -1, __VA_ARGS__));\
									SOF_CRITICAL(message); throw Sofia::MessageException(__LINE__, __FILE__, message, false); } }

#define SOF_CORE_THROW throw Sofia::SofiaException(__LINE__, __FILE__, true);
#define SOF_THROW throw Sofia::SofiaException(__LINE__, __FILE__, false);

#define SOF_CORE_THROW_INFO(message) { SOF_CORE_CRITICAL(message);  throw Sofia::MessageException(__LINE__, __FILE__, message, true); }
#define SOF_THROW_INFO(message) { SOF_CRITICAL(message);  throw Sofia::MessageException(__LINE__, __FILE__, message, false); }

#define SOF_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)

#include "Sofia/Timer.h"

namespace Sofia {

	void InitializeCore();
	void ShutdownCore();
	uint64_t GetTime() noexcept;
}

#include "Sofia/Ref.h"

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ...Args>
Scope<T> CreateScope(Args&& ...args) noexcept
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}
