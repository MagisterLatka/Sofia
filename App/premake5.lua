project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	dependson
	{
		"Sofia"
	}

	files
	{
		"src/**.cpp",
		"src/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"%{wks.location}/Sofia/src",
		"%{wks.location}/Sofia/vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}"
	}

	links
	{
		"Sofia"
	}
		
	postbuildcommands
	{
		"{COPYFILE} \"%{cfg.targetdir}/../Sofia/*\" \"%{cfg.targetdir}\""
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
