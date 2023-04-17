#include "kspch.h"
#include "Window.h"

#ifdef KS_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // KS_PLATFORM_WINDOWS

namespace Kenshin
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef KS_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		return nullptr;
	#endif // DEBUG
	}
}