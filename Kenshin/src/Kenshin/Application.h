#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace Kenshin
{
	class KENSHIN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_IsRunning;
	};

	Application* createApplication(int argc, char** argv);
}
