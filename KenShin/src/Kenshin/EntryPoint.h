#pragma once
#include <iostream>
#include "Log.h"

extern Kenshin::Application* Kenshin::CreateApplication();

int main()
{
	Kenshin::Log::Init();
	Kenshin::Application* app = Kenshin::CreateApplication();
	std::cout << "Kenshin Engine!" << std::endl;
	app->Run();
	delete app;
}