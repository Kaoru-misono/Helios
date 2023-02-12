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
IncludeDir = {}
IncludeDir["GLFW"] = "Helios/support/GLFW/include"
IncludeDir["Glad"] = "Helios/support/Glad/include"
IncludeDir["ImGui"] = "Helios/support/imgui"
IncludeDir["glm"] = "Helios/support/glm"
IncludeDir["stb_image"] = "Helios/support/stb_image"
IncludeDir["assimp"] = "Helios/support/assimp/include"
IncludeDir["spdlog"] = "Helios/support/spdlog/include"

--include path : include ""
include "Helios/support/GLFW"
include "Helios/support/Glad"
include "Helios/support/imgui"
include "Helios/support/assimp"

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
		"%{prj.name}/core/**.cpp",
		"%{prj.name}/support/stb_image/**.h",
		"%{prj.name}/support/stb_image/**.cpp",
		"%{prj.name}/support/glm/glm/**.hpp",
		"%{prj.name}/support/glm/glm/**.inl"
	}

	defines
	{
		--"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/core",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imGui",
		"assimp",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

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

