workspace "Hashsnail-Client"
	architecture "x64"

	configurations
	{
		"Release",
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Hashsnail-Client"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"libs/spdlog/include",
		"libs/asio/asio/include"
	}

	vpaths 
	{
    ["Header Files"] = { "**.h", "**.hpp" },
    ["Source Files"] = {"**.c", "**.cpp"},
	}
	defines {"ASIO_STANDALONE"}

	filter "system:windows"
		cppdialect "c++20"
		systemversion "latest"
		defines { "_WIN32_WINNT=0x0A00"}


	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"



