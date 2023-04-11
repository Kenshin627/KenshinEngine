#pragma once
#include "Application.h"
#include<stdio.h>

extern Kenshin::Application* Kenshin::createApplication(int argc, char** argv);

#ifdef KS_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
	printf("Kenshin Engine");
	Kenshin::Application* app = Kenshin::createApplication(argc, argv);
	app->Run();
	delete app;
}
#endif // KS_PLATFORM_WINDOWS
