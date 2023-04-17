#pragma once
#include "Application.h"
#include "Log.h"

extern Kenshin::Application* Kenshin::createApplication(int argc, char** argv);

#ifdef KS_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
	Kenshin::Log::init();
	KS_CORE_INFO("KenshinEngine!");
	Kenshin::Application* app = Kenshin::createApplication(argc, argv);
	app->Run();
	delete app;
}
#endif
