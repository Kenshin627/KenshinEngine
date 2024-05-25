#include "kspch.h"
#include "Log.h"
#include "Application.h"
#include "Input.h"
#include <glad/glad.h>

namespace Kenshin {

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		KS_CORE_ASSET(!s_Instance, "application has already exits!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer("ImGuiLayer");
		PushOverLayer(m_ImGuiLayer);
		//temp
		float vertices[12] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		uint32_t indices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};

		const char* vertexSource =
			"#version 330 core\n"
			"\n"
			"layout (location = 0) in vec3 a_Pos;\n"
			"void main {\n"
			" gl_Position = vec4(a_Pos, 1.0);\n"
			"}";
		const char* fragmentSource =
			"#version 330 core\n"
			"\n"
			"out vec4 color;\n"			
			"void main(){\n"
			"color = vec4(0.2, 0.5, 0.1, 1.0);\n"
			"}\n";
		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
		m_Shader->Bind();
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(float) * 12));
		m_VertexBuffer->Bind();
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 6));
		m_IndexBuffer->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//temp
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			for (auto& it : m_LayerStack)
			{
				it->OnUpdate();
			}
			m_ImGuiLayer->Begin();
			for (auto& it : m_LayerStack)
			{
				it->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FN(Application::OnWindowClose));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*(--it))->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}