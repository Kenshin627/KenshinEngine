#pragma once
#include "Core.h"
#include <functional>
#include "Events/Event.h"

namespace Kenshin {
	struct WindowProps
	{
		WindowProps(const std::string& title = "Kenhin Engine", uint32_t width = 1280, uint32_t height = 720) : 
			title(title),
			width(width), 
			height(height) {}
		std::string title;
		uint32_t width;
		uint32_t height;
	};

	KENSHIN_API class Window
	{
	public:
		using EventCallback = std::function<void(Event&)>;
		virtual~Window() = default;
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void Init(const WindowProps& props) = 0;
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}