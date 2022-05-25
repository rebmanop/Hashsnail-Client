workspace "Hashsnail-Client"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Hashsnail-Client"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp",
	}

	includedirs
	{
		"libs/spdlog/include",
		"libs/asio/asio/include"
	}

	vpaths 
	{
    ["Header Files"] = { "**.h", "**.hpp" },
    ["Source Files/*"] = {"**.c", "**.cpp"},
	}

	filter "system:windows"
		cppdialect "c++20"
		systemversion "latest"


	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"



