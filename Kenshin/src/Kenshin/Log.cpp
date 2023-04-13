#include "kspch.h"
#include "Log.h"

Kenshin::Ref<spdlog::logger> Kenshin::Log::s_CoreLogger;
Kenshin::Ref<spdlog::logger> Kenshin::Log::s_ClientLogger;

void Kenshin::Log::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("KENSHIN");
	s_CoreLogger->set_level(spdlog::level::trace);
	s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
	s_ClientLogger->set_level(spdlog::level::trace);
}