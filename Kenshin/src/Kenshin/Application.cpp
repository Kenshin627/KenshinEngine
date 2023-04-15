#include "kspch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
#include <glad/gl.h>

namespace Kenshin
{
	Application* Application::s_Instance;
	Application::Application():m_IsRunning(true)
	{
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));
		ImGuiLayer* m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);

		m_VAO.reset(VertexArray::CreateVertexArray());
		float vertices[3 * 7] = { 
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
			 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
			 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0
		};
		unsigned indices[3] = { 0, 1, 2 };
		m_VBO.reset(VertexBuffer::CreateBuffer(vertices, sizeof(vertices)));
		VertexBufferLayout layout = {
			{ "aPosition", ShaderDataType::Float3 },
			{ "aColor", ShaderDataType::Float4 }
		};
		m_VBO->SetLayout(layout);
		m_VAO->AddVertexBuffer(m_VBO);
		m_EBO.reset(IndexBuffer::CreateBuffer(indices, 3));
		m_VAO->SetIndexBuffer(m_EBO);
		m_Shader = CreateScope<Shader>("../Kenshin/resource/shaders/vertex.glsl", "../Kenshin/resource/shaders/fragment.glsl", "");
		m_Shader->Bind();
		
	}
	Application::~Application() {}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent, std::placeholders::_1));
		KS_CORE_INFO(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}
	void Application::Run() 
	{	
		while (m_IsRunning)
		{
			glClearColor(0.2, 0.2, 0.2, 0.2);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer: m_LayerStack)
			{
				layer->OnUpdate();
			}
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_Shader->Bind();
			m_VAO->Bind();
			glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, (const void*)0);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
		overlay->OnAttach();
	}
}