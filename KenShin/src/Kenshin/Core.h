#pragma once

#ifdef KS_BUILD_DLL
	#define KS_API __declspec(dllexport)
#else
	#define KS_API __declspec(dllimport)
#endif