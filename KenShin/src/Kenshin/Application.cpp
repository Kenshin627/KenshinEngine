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
		float vertices[4 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		};

		uint32_t indices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};

		const char* vertexSource = R"(
			#version 330 core
	
			layout (location = 0) in vec3 a_Pos;
			layout (location = 1) in vec4 a_Color;
			out vec4 v_Color;
			void main() {
				v_Color = a_Color;
				gl_Position = vec4(a_Pos, 1.0);
			}
		 )";

		const char* fragmentSource = R"(
			#version 330 core
			
			out vec4 color;	
			in  vec4 v_Color;
			void main(){
			 color = v_Color;
			};
		)";
		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
		m_Shader->Bind();
		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::Create(vertices, sizeof(float) * 4 * 7));
		std::shared_ptr<IndexBuffer> ibo;
		ibo.reset(IndexBuffer::Create(indices, 6));
		
		BufferLayout layout = 
		{
			{ "a_Position", ShaderDataType::Float3, false },
			{ "a_Color", ShaderDataType::Float4, false }
		};
		vbo->SetLayout(layout);
		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vbo);
		m_VertexArray->SetIndeBuffer(ibo);
		m_VertexArray->Bind();
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
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
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