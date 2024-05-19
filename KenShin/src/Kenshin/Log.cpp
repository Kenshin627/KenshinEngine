#include "Log.h"

namespace Kenshin {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		spdlog::set_level(spdlog::level::trace);
		s_CoreLogger = spdlog::stdout_color_mt("CORE");
		s_ClientLogger = spdlog::stdout_color_mt("APP");
	}
}
