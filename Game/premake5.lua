project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	characterset ("MBCS")

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	dependson
	{
		"Sofia"
	}

	files
	{
		"src/**.cpp",
		"src/**.h",
		"assets/shaders/**.hlsl"
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
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.entt}"
	}

	links
	{
		"Sofia"
	}
	
	buildoptions
	{
		"/Zc:preprocessor"
	}

	postbuildcommands
	{
		"{COPY} \"assets\" \"%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Game/assets\""
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
	
	filter "files:**.vert.hlsl"
		shadertype ("Vertex")
		shaderobjectfileoutput ("%{prj.location}/assets/shaders/%{file.basename}.cso")
		
	filter "files:**.frag.hlsl"
		shadertype ("Pixel")
		shaderobjectfileoutput ("%{prj.location}/assets/shaders/%{file.basename}.cso")
