#pragma once
#include "Core.h"

namespace Kenshin{

	class KS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run() const;
	};
	Application* CreateApplication();
}

