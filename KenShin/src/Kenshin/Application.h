#pragma once
#include "Core.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Kenshin/imgui/ImGuiLayer.h"
#include "Kenshin/Renderer/Buffer.h"
#include "Kenshin/Renderer/Shader.h"
#include "Kenshin/Renderer/VertexArray.h"
#include "Kenshin/Renderer/OrthographicCamera.h"

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
		//temp
		std::shared_ptr<Shader>             m_Shader;
		std::shared_ptr<VertexArray>        m_VertexArray;
		std::shared_ptr<OrthographicCamera> m_Camera;
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

