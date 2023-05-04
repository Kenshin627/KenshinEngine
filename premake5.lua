workspace "KenshinEngine"
	architecture "x64"
	startproject "KenshinEditor"
	configurations{ "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kenshin/vendor/GLFW/include"
IncludeDir["Glad"] = "Kenshin/vendor/Glad/include"
IncludeDir["ImGui"] = "Kenshin/vendor/ImGui"
IncludeDir["glm"] = "Kenshin/vendor/glm/glm"
IncludeDir["stb_image"] = "Kenshin/vendor/stb_image"
IncludeDir["entt"] = "Kenshin/vendor/entt/include"
IncludeDir["ImGuizmo"] = "Kenshin/vendor/ImGuizmo"
IncludeDir["box2d"] = "Kenshin/vendor/box2d"

include "Kenshin/vendor/GLFW"
include "Kenshin/vendor/Glad"
include "Kenshin/vendor/ImGui"
include "Kenshin/vendor/box2d"

project "KenshinEditor"
	location "KenshinEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-in/" .. outputdir .. "/%{prj.name}")

	files{ "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs
	{ 
		"Kenshin/vendor/spdlog/include;",
		"Kenshin/src;",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Kenshin"
	}

	filter "system:windows"
		cppdialect "c++17"
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

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-in/" .. outputdir .. "/%{prj.name}")

	files{ "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs
	{ 
		"Kenshin/vendor/spdlog/include;",
		"Kenshin/src;",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Kenshin"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
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
	kind "StaticLib"
	language "c++"
	staticruntime "off"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-in/" .. outputdir .. "/%{prj.name}")

	pchheader "kspch.h"
	pchsource ("%{prj.name}/src/kspch.cpp")

	files
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" ,
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/entt/include/**.hpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
	}
	

	includedirs
	{ 
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.box2d}"
	}


	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"imgui",
		"box2d"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }


	filter "system:windows"
		cppdialect "c++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"KS_PLATFORM_WINDOWS",
			"KS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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

	filter "files:vendor/**.cpp"
		flags { "NoPCH" }


	filter "files:src/Platform/OpenGLimgui_impl_glfw.cpp"
		flags { "NoPCH" }

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }
