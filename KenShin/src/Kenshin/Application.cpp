#include "kspch.h"
#include "Application.h"
#include "events/ApplicationEvent.h"
#include "Log.h"
#include <GLFW/glfw3.h>

namespace Kenshin {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}