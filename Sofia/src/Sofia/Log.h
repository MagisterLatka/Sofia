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

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/ostream.h>

namespace Sofia {

	class Log
	{
	public:
		SOF_CORE static void Init();

		SOF_CORE static std::shared_ptr<spdlog::logger> GetCoreLogger() noexcept { return s_CoreLogger; }
		SOF_CORE static std::shared_ptr<spdlog::logger> GetClientLogger() noexcept { return s_ClientLogger; }
	private:
		SOF_CORE static std::shared_ptr<spdlog::logger> s_CoreLogger;
		SOF_CORE static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

	inline std::string Format(const std::string& text, int argNum) noexcept
	{
		return text;
	}
	template<typename T>
	std::string Format(const std::string& text, int argNum, T&& arg) noexcept
	{
		std::ostringstream oss;
		size_t a = text.find('{' + std::to_string(argNum) + '}');
		size_t b = 0;
		while (a != std::string::npos)
		{
			oss << text.substr(b, a - b);
			oss << arg;
			b = a + 2 + std::to_string(argNum).length();
			a = text.find('{' + std::to_string(argNum) + '}', b);
		}
		if (b < text.length()) oss << text.substr(b);
		std::string output = oss.str();
		return output;
	}
	template<typename T, typename ...Args>
	std::string Format(const std::string& text, int argNum, T&& arg, Args&& ...args) noexcept
	{
		std::ostringstream oss;
		size_t a = text.find('{' + std::to_string(argNum) + '}');
		size_t b = 0;
		while (a != std::string::npos)
		{
			oss << text.substr(b, a - b);
			oss << arg;
			b = a + 2 + std::to_string(argNum).length();
			a = text.find('{' + std::to_string(argNum) + '}', b);
		}
		if (b < text.length()) oss << text.substr(b);
		std::string output = oss.str();
		return Format(output, ++argNum, std::forward<Args>(args)...);
	}
}

#define SOF_CORE_TRACE(...)		Sofia::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SOF_CORE_INFO(...)		Sofia::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SOF_CORE_WARN(...)		Sofia::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SOF_CORE_ERROR(...)		Sofia::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SOF_CORE_CRITICAL(...)	Sofia::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SOF_TRACE(...)			Sofia::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SOF_INFO(...)			Sofia::Log::GetClientLogger()->info(__VA_ARGS__)
#define SOF_WARN(...)			Sofia::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SOF_ERROR(...)			Sofia::Log::GetClientLogger()->error(__VA_ARGS__)
#define SOF_CRITICAL(...)		Sofia::Log::GetClientLogger()->critical(__VA_ARGS__)
