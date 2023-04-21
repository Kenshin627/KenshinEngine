#pragma once
#include <memory>
#include <functional>

#ifdef KS_PLATFORM_WINDOWS
	#ifdef KS_BUILD_DLL
		#define KENSHIN_API __declspec(dllexport)
	#else
		#define KENSHIN_API __declspec(dllimport)
	#endif

#endif

#define BIT(x) (1 << x)

#ifdef KS_ENABLE_ASSERTS
	#define KS_ASSERT(x, ...) { if(!(x)) { KS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define KS_CORE_ASSERT(x, ...) { if(!(x)) { KS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define KS_ASSERT(x, ...)
	#define KS_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(x, ...) std::bind(&x, this, __VA_ARGS__)


namespace Kenshin
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename...Args>
	constexpr Scope<T> CreateScope(Args&&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename...Args>
	constexpr Ref<T> CreateRef(Args&&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
