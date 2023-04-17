#pragma once
#include "Kenshin/Core/Window.h"
#include "Kenshin/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Kenshin
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate() override;
		uint32_t GetWidth() const override { return m_Data.Width; };
		uint32_t GetHeight() const override { return m_Data.Height; };
		void SetEventCallback(const EventCallback& callback) override { m_Data.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void* GetNativeWindow() const override;
	private:
		void Init(const WindowProps& props);
		void ShutDown();
	private:
		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool IsVSync;
			EventCallback EventCallback;
		};
		GLFWwindow* m_Window;
		WindowData m_Data;
		GraphicsContext* m_Context;
	};
}
