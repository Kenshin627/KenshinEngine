#include <Kenshin.h>
#include "imgui.h"
#include <glm.hpp>

class ExampleLayer :public Kenshin::Layer
{
public:
	ExampleLayer() :Layer("Example") 
	{
		m_VAO.reset(Kenshin::VertexArray::CreateVertexArray());
		float vertices[3 * 7] = {
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
			 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
			 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0
		};
		unsigned indices[3] = { 0, 1, 2 };
		m_VBO.reset(Kenshin::VertexBuffer::CreateBuffer(vertices, sizeof(vertices)));
		Kenshin::VertexBufferLayout layout = 
		{
			{ "aPosition", Kenshin::ShaderDataType::Float3 },
			{ "aColor",    Kenshin::ShaderDataType::Float4 }
		};
		m_VBO->SetLayout(layout);
		m_VAO->AddVertexBuffer(m_VBO);
		m_EBO.reset(Kenshin::IndexBuffer::CreateBuffer(indices, 3));
		m_VAO->SetIndexBuffer(m_EBO);
		m_Shader = Kenshin::CreateScope<Kenshin::Shader>("../Kenshin/resource/shaders/vertex.glsl", "../Kenshin/resource/shaders/fragment.glsl", "");

		m_Camera = Kenshin::CreateRef<Kenshin::OrthographicCamera>(-2.0f, 2.0f, -2.0f, 2.0f, 0.05f);
		m_Camera->SetRotation(45.0f);
		m_Camera->SetPosition({ 1.0f, 1.0f, 0.0f });
	}
	void  OnUpdate() override
	{
		UpdateCamera();
		Kenshin::Renderer::BeginScene(m_Camera);
		Kenshin::Renderer::Submit(m_VAO, m_Shader);
		Kenshin::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
	}

	void UpdateCamera()
	{
		if (Kenshin::Input::IsKeyPressed(Kenshin::Key::W))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera->GetSpeed());
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::S))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera->GetSpeed());
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::A))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * m_Camera->GetSpeed());
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::D))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::vec3(1.0f, 0.0f, 0.0f) * m_Camera->GetSpeed());
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::Q))
		{
			m_Camera->SetRotation(m_Camera->GetRotation() + 1.0f);
		}

		else if (Kenshin::Input::IsKeyPressed(Kenshin::Key::E))
		{
			m_Camera->SetRotation(m_Camera->GetRotation() - 1.0f);
		}
	}
private:
	Kenshin::Ref<Kenshin::VertexBuffer> m_VBO;
	Kenshin::Ref<Kenshin::IndexBuffer> m_EBO;
	Kenshin::Ref<Kenshin::VertexArray> m_VAO;
	Kenshin::Ref<Kenshin::Shader> m_Shader;
	Kenshin::Ref<Kenshin::OrthographicCamera> m_Camera;
};

class SandBox :public Kenshin::Application
{
public:
	SandBox() 
	{
		PushOverLay(new ExampleLayer());
	}
	~SandBox() {};
};

Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	return new SandBox();
}