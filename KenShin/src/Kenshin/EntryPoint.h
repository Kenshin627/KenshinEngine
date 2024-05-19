#pragma once
#include "Log.h"

extern Kenshin::Application* Kenshin::CreateApplication();

int main()
{
	Kenshin::Log::Init();
	Kenshin::Application* app = Kenshin::CreateApplication();
	app->Run();
	delete app;
}