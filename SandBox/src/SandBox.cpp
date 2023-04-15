#include <Kenshin.h>
#include "imgui.h"
#include <glm.hpp>
#include "Kenshin/Renderer/Renderer.h"

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
		m_Shader->Bind();
	}
	void  OnUpdate() override
	{
		Kenshin::Renderer::BeginScene();
		m_Shader->Bind();
		Kenshin::Renderer::Submit(m_VAO);
		Kenshin::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
	}
private:
	Kenshin::Ref<Kenshin::VertexBuffer> m_VBO;
	Kenshin::Ref<Kenshin::IndexBuffer> m_EBO;
	Kenshin::Ref<Kenshin::VertexArray> m_VAO;
	Kenshin::Ref<Kenshin::Shader> m_Shader;
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