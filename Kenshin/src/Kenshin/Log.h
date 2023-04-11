#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

namespace Kenshin
{
	class KENSHIN_API Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger> getCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger> getClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}



#define KS_CORE_TRACE(...)    ::Kenshin::Log::getCoreLogger()->trace(__VA_ARGS__);
#define KS_CORE_INFO(...)     ::Kenshin::Log::getCoreLogger()->info(__VA_ARGS__);
#define KS_CORE_WARN(...)     ::Kenshin::Log::getCoreLogger()->warn(__VA_ARGS__);
#define KS_CORE_CRITICAL(...) ::Kenshin::Log::getCoreLogger()->critical(__VA_ARGS__);

#define KS_CLIENT_TRACE(...)    ::Kenshin::Log::getClientLogger()->trace(__VA_ARGS__);
#define KS_CLIENT_INFO(...)     ::Kenshin::Log::getClientLogger()->info(__VA_ARGS__);
#define KS_CLIENT_WARN(...)     ::Kenshin::Log::getClientLogger()->warn(__VA_ARGS__);
#define KS_CLIENT_CRITICAL(...) ::Kenshin::Log::getClientLogger()->critical(__VA_ARGS__);

