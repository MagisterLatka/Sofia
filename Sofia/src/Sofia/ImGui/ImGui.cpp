#include <pch.h>

#if defined(SOF_PLATFORM_WINDOWS)
#	include <backends/imgui_impl_win32.cpp>
#	include <backends/imgui_impl_dx11.cpp>
//#	include <backends/imgui_impl_dx12.cpp>
#else
#	include <backends/imgui_impl_glfw.cpp>
#endif

#include <backends/imgui_impl_opengl3.cpp>
//#include <backends/imgui_impl_vulkan.cpp>
