workspace "KenshinEngine"
	architecture "x64"
	startproject "SandBox"
	configurations{ "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kenshin/vendor/GLFW/include"

include "Kenshin/vendor/GLFW"

project "SandBox"
	location "SandBox"
	kind "consoleApp"
	language "c++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-in/" .. outputdir .. "/%{prj.name}")

	files{ "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs
	{ 
		"Kenshin/vendor/spdlog/include;",
		"Kenshin/src;"
	}

	links
	{
		"Kenshin"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"KS_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "KS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "KS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KS_DIST"
		optimize "On"

project "Kenshin"
	location "Kenshin"
	kind "SharedLib"
	language "c++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-in/" .. outputdir .. "/%{prj.name}")

	pchheader "kspch.h"
	pchsource ("%{prj.name}/src/kspch.cpp")

	files{ "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	

	includedirs
	{ 
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}"
	}
	postbuildcommands
	{
		"{COPY} %{cfg.buildtarget.relpath} ../bin/"  .. outputdir .. "/SandBox"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"KS_PLATFORM_WINDOWS",
			"KS_BUILD_DLL"
		}

		

	filter "configurations:Debug"
		defines 
		{ 
			"KS_DEBUG",
			"KS_ENABLE_ASSERTS"
		}
		symbols "On"

	filter "configurations:Release"
		defines "KS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KS_DIST"
		optimize "On"

	filter "files: vendor/**.cpp"
	flags { "NoPCH" }

