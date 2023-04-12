#pragma once
#include "Core.h"
#include "Window.h"

namespace Kenshin
{
	class KENSHIN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		Scope<Window> m_Window;
		bool m_IsRunning;
	};

	Application* createApplication(int argc, char** argv);
}
