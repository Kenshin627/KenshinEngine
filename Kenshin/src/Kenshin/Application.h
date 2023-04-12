#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

namespace Kenshin
{
	class KENSHIN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_IsRunning;
		LayerStack m_LayerStack;
	};

	Application* createApplication(int argc, char** argv);
}
