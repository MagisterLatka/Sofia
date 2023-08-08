include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Sofia"
	startproject "Game"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	solution_items
	{
		".editorconfig"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dep"
	include "vendor/premake"
group ""

group "Core"
include "Sofia"
group ""

include "Game"
