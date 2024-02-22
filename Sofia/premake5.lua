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

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"SOF_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
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
		"GLFW",
		"GLAD"
	}	

	filter "system:windows"
		systemversion "latest"
		
		buildoptions
		{
			"/Zc:preprocessor"
		}
		
		links
		{
			"d3d11.lib",
			"D3DCompiler.lib",
			"dxguid.lib",
			"opengl32.lib"
		}
		
	filter "system:linux"
		pic "On"
		
		removefiles
		{
			"src/Sofia/Platform/DX11/**.cpp",
			"src/Sofia/Platform/DX11/**.h",
			"src/Sofia/Platform/Windows/**.cpp",
			"src/Sofia/Platform/Windows/**.h"
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
