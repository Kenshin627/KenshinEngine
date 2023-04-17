#include "kspch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
#include "../Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "TimeStamp.h"

namespace Kenshin
{
	Application* Application::s_Instance;
	Application::Application():m_IsRunning(true), m_LastFrameTime(0.0f)
	{
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));

		Renderer::Init();

		ImGuiLayer* m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);		
	}
	Application::~Application() {}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize, std::placeholders::_1));
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
			float time = (float)glfwGetTime();
			TimeStamp ts{ time - m_LastFrameTime };
			m_LastFrameTime = time;
		
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(ts);
			}						
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		size_t width = (float)e.GetWidth();
		size_t height = (float)e.GetHeight();
		Renderer::OnWindowResize(width, height);
		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
		overlay->OnAttach();
	}
}