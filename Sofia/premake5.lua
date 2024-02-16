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
		"SOF_BUILD_DLL"
	}
		
	includedirs
	{
		"src",
		"vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}"
	}

	links
	{
		
	}
	
	

	filter "system:windows"
		systemversion "latest"
		buildoptions
		{
			"/Zc:preprocessor"
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
