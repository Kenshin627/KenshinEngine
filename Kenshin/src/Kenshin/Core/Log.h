#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Kenshin
{
	class  Log
	{
	public:
		static void init();
		inline static Ref<spdlog::logger>& getCoreLogger() { return s_CoreLogger; };
		inline static Ref<spdlog::logger>& getClientLogger() { return s_ClientLogger; };
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}


//core log macro
#define KS_CORE_TRACE(...)      ::Kenshin::Log::getCoreLogger()->trace(__VA_ARGS__);
#define KS_CORE_INFO(...)       ::Kenshin::Log::getCoreLogger()->info(__VA_ARGS__);
#define KS_CORE_WARN(...)       ::Kenshin::Log::getCoreLogger()->warn(__VA_ARGS__);
#define KS_CORE_ERROR(...)      ::Kenshin::Log::getCoreLogger()->error(__VA_ARGS__);
#define KS_CORE_CRITICAL(...)   ::Kenshin::Log::getCoreLogger()->critical(__VA_ARGS__);

//client log macro
#define KS_TRACE(...)           ::Kenshin::Log::getClientLogger()->trace(__VA_ARGS__);
#define KS_INFO(...)            ::Kenshin::Log::getClientLogger()->info(__VA_ARGS__);
#define KS_WARN(...)            ::Kenshin::Log::getClientLogger()->warn(__VA_ARGS__);
#define KS_ERROR(...)           ::Kenshin::Log::getClientLogger()->error(__VA_ARGS__);
#define KS_CRITICAL(...)        ::Kenshin::Log::getClientLogger()->critical(__VA_ARGS__);

