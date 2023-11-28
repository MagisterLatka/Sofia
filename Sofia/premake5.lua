project "Sofia"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	characterset ("MBCS")

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.cpp",
		"src/**.h",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
		
	includedirs
	{
		"src",
		"vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.yaml}",
		"%{IncludeDirs.mono}"
	}

	links
	{
		"ImGui",
		"d3d11.lib",
		"D3DCompiler.lib",
		"dxguid.lib",
		"yaml-cpp",
		"%{Library.mono}"
	}
	
	buildoptions
	{
		"/Zc:preprocessor"
	}

	filter "system:windows"
		systemversion "latest"
		
		links
		{
			"%{Library.WinSocket}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}"
		}
		
	filter "configurations:Debug"
		defines "SOF_DEBUG"
		symbols "On"
		runtime "Debug"

		links
		{
		
		}

	filter "configurations:Release"
		defines "SOF_RELEASE"
		optimize "Full"
		runtime "Release"
		inlining "Auto"

		links
		{
		

		}