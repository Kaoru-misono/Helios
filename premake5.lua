workspace "Helios"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
--IncludeDir = {}

--include path : include ""

project "Helios"
	location "Helios"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "%{prj.name}")
	objdir("bin-int/" .. outputdir .. "%{prj.name}")

	--pchheader
	--pchsource

	files
	{
		"%{prj.name}/core/**.hpp",
		"%{prj.name}/core/**.cpp"
	}

	defines
	{
		--"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/core"
	}

	links
	{
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		--defines "Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/runtime/**.hpp",
		"%{prj.name}/runtime/**.cpp",
		"%{prj.name}/source/**.hpp",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"Helios/core",
		"Helios/support"
	}

	links
	{
		"Helios"
	}

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

