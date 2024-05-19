#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Core.h"

namespace Kenshin {
	class KS_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//kenshin core logger
#define KS_CORE_TRACE(...)     ::Kenshin::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KS_CORE_DEBUG(...)     ::Kenshin::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define KS_CORE_INFO(...)      ::Kenshin::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KS_CORE_WARN(...)      ::Kenshin::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KS_CORE_CRITICAL(...)  ::Kenshin::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define KS_CORE_ERROR(...)     ::Kenshin::Log::GetCoreLogger()->error(__VA_ARGS__)

//kenshin client logger
#define KS_CLIENT_TRACE(...)   ::Kenshin::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KS_CLIENT_DEBUG(...)   ::Kenshin::Log::GetClientLogger()->debug(__VA_ARGS__)
#define KS_CLIENT_INFO(...)    ::Kenshin::Log::GetClientLogger()->info(__VA_ARGS__)
#define KS_CLIENT_WARN(...)    ::Kenshin::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KS_CLIENT_CRITICAL(...)::Kenshin::Log::GetClientLogger()->critical(__VA_ARGS__)
#define KS_CLIENT_ERROR(...)   ::Kenshin::Log::GetClientLogger()->error(__VA_ARGS__)
