#pragma once

#ifdef KS_PLATFORM_WINDOWS
	#ifdef KS_BUILD_DLL
		#define KENSHIN_API __declspec(dllexport)
	#else
		#define KENSHIN_API __declspec(dllimport)
	#endif // DEBUG

#endif // PLATFORM_WINDW

#define BIT(x) (1 << x)