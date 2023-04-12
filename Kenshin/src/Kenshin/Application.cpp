#include "kspch.h"
#include "Application.h"
#include "Log.h"
#include <glad/gl.h>

namespace Kenshin
{
	Application::Application():m_IsRunning(true)
	{
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	Application::~Application() {}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
		KS_CORE_INFO(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}
	void Application::Run() 
	{	
		while (m_IsRunning)
		{
			glClearColor(0.2, 0.2, 0.2, 0.2);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer: m_LayerStack)
			{
				layer->OnUpdate();
			}
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
	}
}