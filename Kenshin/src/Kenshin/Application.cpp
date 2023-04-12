#include "kspch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include "GLFW/glfw3.h"

namespace Kenshin
{
	Application::Application():m_IsRunning(true)
	{
		m_Window = Window::Create();
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}
	Application::~Application() {}
	void Application::OnEvent(const Event& e)
	{
		KS_CORE_INFO(e);
	}
	void Application::Run() 
	{	
		while (m_IsRunning)
		{
			glClearColor(0.2, 0.2, 0.2, 0.2);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}