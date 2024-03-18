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
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.entt}"
	}

	links
	{
		"ImGui",
		"Sofia"
	}

	postbuildcommands
	{
		"{COPYDIR} \"%{prj.location}/assets\" \"%{cfg.targetdir}/assets\""
	}
	
	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"assets/shaders/**.hlsl"
		}
		
		buildoptions
		{
			"/Zc:preprocessor"
		}
		postbuildcommands
		{
			"{COPYFILE} \"%{cfg.targetdir}/../Sofia/Sofia.dll\" \"%{cfg.targetdir}\""
		}

	filter "system:linux"
		pic "On"
		
		postbuildcommands
		{
			"{COPYFILE} \"%{cfg.targetdir}/../Sofia/libSofia.so\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Debug"
		defines "SOF_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "SOF_RELEASE"
		optimize "Full"
		runtime "Release"

	filter "files:**.vert.hlsl"
		shadertype ("Vertex")
		shaderobjectfileoutput ("%{prj.location}/assets/shaders/%{file.basename}.cso")

	filter "files:**.frag.hlsl"
		shadertype ("Pixel")
		shaderobjectfileoutput ("%{prj.location}/assets/shaders/%{file.basename}.cso")
