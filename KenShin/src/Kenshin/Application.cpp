#include "kspch.h"
#include "Application.h"
#include "Log.h"
#include <GLFW/glfw3.h>


namespace Kenshin {

#define BIND_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_FN(OnEvent));
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FN(OnWindowClose));
		KS_CORE_TRACE(e.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}