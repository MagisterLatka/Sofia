project "Sofia"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.cpp",
		"src/**.h"
	}
	
	removefiles
	{
		"src/Sofia/Platform/**"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"SOF_BUILD_DLL"
	}
		
	includedirs
	{
		"src",
		"vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.glfw}",
		"%{IncludeDirs.glad}"
	}

	links
	{
		
	}	

	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"src/Sofia/Platform/DX11/**.cpp",
			"src/Sofia/Platform/DX11/**.h",
			"src/Sofia/Platform/Windows/**.cpp",
			"src/Sofia/Platform/Windows/**.h"
		}
		
		buildoptions
		{
			"/Zc:preprocessor"
		}
		
		links
		{
			"d3d11.lib",
			"D3DCompiler.lib",
			"dxguid.lib"
		}
		
	filter "system:linux"
		pic "On"
		
		files
		{
			"src/Sofia/Platform/Linux/**.cpp",
			"src/Sofia/Platform/Linux/**.h",
			"src/Sofia/Platform/OpenGL/**.cpp",
			"src/Sofia/Platform/OpenGL/**.h"
		}
		
		links
		{
			"GLFW",
			"GLAD"
		}

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "SOF_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "SOF_RELEASE"
		optimize "Full"
		runtime "Release"
		inlining "Auto"
