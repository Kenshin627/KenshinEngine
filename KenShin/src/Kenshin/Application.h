#pragma once
#include "Core.h"
#include "Window.h"

namespace Kenshin{

	class KS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};
	Application* CreateApplication();
}
