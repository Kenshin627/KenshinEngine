#pragma once
#include "Core.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "LayerStack.h"

namespace Kenshin{

	class KS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overlay);
	private:
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};
	Application* CreateApplication();
}

