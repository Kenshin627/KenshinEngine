#pragma once
#include "Core.h"

namespace Kenshin
{
	class KENSHIN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	__declspec(dllexport) Application* createApplication(int argc, char** argv);
}
