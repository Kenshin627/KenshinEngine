project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/gl.h",
		"include/KHR/khrplatform.h",
		"src/gl.c"
	}

	includedirs
	{
		"include"
	}

	
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"