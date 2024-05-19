#pragma once
#include "Kenshin/Window.h"
#include <GLFW/glfw3.h>

namespace Kenshin {
	class KS_API Windowwindow :public Window
	{
	public:
		Windowwindow(const WindowProps& props);
		virtual ~Windowwindow();
		virtual void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width;  }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void SetEventCallback(const EventFn& fn) override { m_Data.Callback = fn; };
		virtual void SetVSync(bool enable) override;
		virtual bool IsVSync() const override { return m_Data.IsVSync; };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			const char* Title;
			unsigned int Width, Height;
			bool IsVSync;
			EventFn Callback;
		};
		WindowData m_Data;
	};
}