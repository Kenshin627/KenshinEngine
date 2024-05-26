#pragma once
#include "Core.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Kenshin/imgui/ImGuiLayer.h"

namespace Kenshin{

	class KS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overlay);
		static Application* Get() { return s_Instance; }
		Window& GetWindow() { return *m_Window; }
	private:
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		float m_LastFrametime = 0.0f;
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

