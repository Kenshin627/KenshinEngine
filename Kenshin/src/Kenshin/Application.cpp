#include "kspch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Kenshin
{
	Application::Application():m_IsRunning(true)
	{
		m_Window = Window::Create();
	}
	Application::~Application() {}
	void Application::Run() 
	{	
		while (m_IsRunning)
		{
			m_Window->OnUpdate();
		}
	}
}