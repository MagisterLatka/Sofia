local rootDir = '../../../'
include (rootDir .. "vendor/premake/premake_customization/solution_items.lua")

workspace "Game"
	startproject "Game"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Game"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	
	targetdir ("Binaries")
	objdir ("Intermediates")
	
	files
	{
		"Source/**.cs"
	}
	
	links
	{
		"Sofia-ScriptCore"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
	
group "Sofia"
	include (rootDir .. "Sofia-ScriptCore")
group ""
	