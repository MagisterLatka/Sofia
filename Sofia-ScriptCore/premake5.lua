project "Sofia-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	
	targetdir ("../Game/assets/scripts")
	objdir ("../Game/assets/scripts/Intermediates")
	
	files
	{
		"Source/**.cs",
		"Properties/**cs"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "Full"
		symbols "Off"
