#pragma once
#include "Core.h"
#include "Window.h"
#include "../Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Kenshin/ImGuiLayer/ImGuiLayer.h"
#include "Kenshin/Renderer/Buffer.h"
#include "Kenshin/Renderer/Shader.h"
#include "../Renderer/VertexArray.h"

namespace Kenshin
{
	class KENSHIN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		static Application& Get() { return *s_Instance; };
		Window& GetWindow() const { return *m_Window; };
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning;
		LayerStack m_LayerStack;
		static Application* s_Instance;
		float m_LastFrameTime;
	};

	Application* createApplication(int argc, char** argv);
}
