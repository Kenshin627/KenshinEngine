#include "kspch.h"
#include "Log.h"
#include "Application.h"
#include "Input.h"
#include "Kenshin/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Kenshin {

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		KS_CORE_ASSET(!s_Instance, "application has already exits!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer("ImGuiLayer");
		PushOverLayer(m_ImGuiLayer);

		Renderer::Init();
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			float time = glfwGetTime();
			Timestep ts = time - m_LastFrametime;
			m_LastFrametime = time;
			for (auto& it : m_LayerStack)
			{
				it->OnUpdate(ts);
			}
			m_ImGuiLayer->Begin();
			for (auto& it : m_LayerStack)
			{
				it->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FN(Application::OnWindowClose));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*(--it))->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}