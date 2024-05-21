#pragma once
#include "kspch.h"
#include "events/Event.h"

namespace Kenshin {
	struct WindowProps
	{
		const char* Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const char* title = "Window", unsigned int width = 1920, unsigned int height = 1080) 
			: Title(title), 
			Width(width), 
			Height(height) 
		{}
	};

	class KS_API Window
	{
	public:
		using EventFn = std::function<void(Event&)>;
	public:
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetEventCallback(const EventFn& fn) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* NativeWindow() = 0;
		static Window* Create(const WindowProps& props = WindowProps());
	};
}