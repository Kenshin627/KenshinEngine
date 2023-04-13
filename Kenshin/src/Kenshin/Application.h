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
		static Application& Get() { return *s_Instance; };
		Window& GetWindow() const { return *m_Window; };
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_IsRunning;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	Application* createApplication(int argc, char** argv);
}
