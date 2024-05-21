#pragma once

#ifdef KS_BUILD_DLL
	#define KS_API __declspec(dllexport)
#else
	#define KS_API __declspec(dllimport)
#endif

#ifdef KE_ENABLE_ASSETS
#define KS_ASSET(x, ...) { if(!(x)) { KS_CLINET_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define KS_CORE_ASSET(x, ...) { if(!(x)) { KS_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak; } }
#else
#define KS_ASSET(x, ...)
#define KS_CORE_ASSET(x, ...)
#endif

#define BIT(x) (1 << x)