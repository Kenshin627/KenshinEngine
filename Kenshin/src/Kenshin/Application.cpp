#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Kenshin
{
	Application::Application() {}
	Application::~Application() {}
	void Application::Run() 
	{
		auto e = WindowResizeEvent(1920, 1080);
		KS_CORE_TRACE(e);
		while (true)
		{

		}
	}
}