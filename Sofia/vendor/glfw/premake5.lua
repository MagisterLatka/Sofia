project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"include/**.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/null_init.c",
		"src/null_joystick.c",
		"src/null_monitor.c",
		"src/null_window.c",
		"src/platform.c",
		"src/monitor.c",
		"src/vulkan.c",
		"src/window.c",
	}
	
	filter "system:linux"
		pic "On"
		
		files
		{
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/xkb_unicode.c",
			"src/posix_module.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/posix_module.c",
			"src/posix_poll.c",
			"src/glx_context.c",
			"src/egl_context.c",
			"src/osmesa_context.c",
			"src/linux_joystick.c"
		}
		
		defines
		{
			"_GLFW_X11"
		}
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "Full"

	
