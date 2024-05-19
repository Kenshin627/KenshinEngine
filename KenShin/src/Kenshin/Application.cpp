#include "Application.h"
#include "events/ApplicationEvent.h"
#include "Log.h"
#include <iostream>

namespace Kenshin {
	Application::Application()
	{

	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		WindowResizeEvent e(100, 100);

		KS_CORE_ERROR(e.ToString());
		while (true);
	}
}