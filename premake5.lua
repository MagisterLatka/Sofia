include "Dependencies.lua"

workspace "Sofia"
	startproject "App"
	architecture "x86_64"

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

group "Dep"
group ""

group "Core"
	include "Sofia"
group ""

include "App"
