#if defined(_WIN32)
#	if defined(_WIN64)
#		define SOF_PLATFORM_WINDOWS
#else
#		error "x86 Builds are not supported!"
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error "IOS simulator is not supported!"
#	elif TARGET_OS_IPHONE == 1
#		define SOF_PLATFORM_IOS
#		error "IOS is not supported!"
#	elif TARGET_OS_MAC == 1
#		define SOF_PLATFORM_MACOS
#		error "MacOS is not supported!"
#	else
#		error "Unknown Apple platform!"
#	endif
#elif defined(__ANDROID__)
#	define SOF_PLATFORM_ANDROID
#	error "Android is not supported!"
#elif defined(__linux__)
#	define SOF_PLATFORM_LINUX
#	error "Linux is not supported!"
#else
#	error "Unknown platform!"
#endif

#if defined(SOF_DEBUG)
#	if defined(SOF_PLATFORM_WINDOWS)
#		define SOF_BREAK() __debugbreak()
#	elif defined (SOF_PLATFORM_LINUX)
#		include <signal.h>
#		define SOF_BREAK() raise(SIGTRAP)
#	else
#		error "Platform does not support debug break!"
#	endif
#else
#	define SOF_BREAK()
#endif
