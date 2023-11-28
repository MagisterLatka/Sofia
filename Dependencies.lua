IncludeDirs = {}
IncludeDirs["spdlog"] = "%{wks.location}/Sofia/vendor/spdlog/include"
IncludeDirs["glm"] = "%{wks.location}/Sofia/vendor/glm"
IncludeDirs["imgui"] = "%{wks.location}/Sofia/vendor/imgui"
IncludeDirs["stb_image"] = "%{wks.location}/Sofia/vendor/stb_image"
IncludeDirs["entt"] = "%{wks.location}/Sofia/vendor/entt"
IncludeDirs["yaml"] = "%{wks.location}/Sofia/vendor/yaml-cpp/include"
IncludeDirs["mono"] = "%{wks.location}/Sofia/vendor/mono/include"

LibraryDirs = {}
LibraryDirs["mono"] = "%{wks.location}/Sofia/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDirs.mono}/libmono-static-sgen.lib"

Library["WinSocket"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
