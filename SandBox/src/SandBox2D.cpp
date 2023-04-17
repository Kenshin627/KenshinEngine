#include "kspch.h"
#include "SandBox2D.h"
#include <imgui.h>

SandBox2D::SandBox2D() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D") {}

void SandBox2D::OnAttach()
{
	#pragma region QUAD
	m_QUADVAO.reset(Kenshin::VertexArray::CreateVertexArray());
	float quadVertices[4 * 3] = {
		-0.5,  0.5, 0.0, 
		 0.5,  0.5, 0.0, 
		-0.5, -0.5, 0.0, 
		 0.5, -0.5, 0.0 	
	};

	unsigned quadIndices[6] = { 0, 1, 2, 1, 2, 3 };
	Kenshin::Ref<Kenshin::VertexBuffer> quadVBO = Kenshin::Ref<Kenshin::VertexBuffer>(Kenshin::VertexBuffer::CreateBuffer(quadVertices, sizeof(quadVertices)));
	Kenshin::VertexBufferLayout quadLayout =
	{
		{ "aPosition", Kenshin::ShaderDataType::Float3 }
	};
	quadVBO->SetLayout(quadLayout);
	m_QUADVAO->AddVertexBuffer(quadVBO);
	Kenshin::Ref<Kenshin::IndexBuffer> quadEBO = Kenshin::Ref<Kenshin::IndexBuffer>(Kenshin::IndexBuffer::CreateBuffer(quadIndices, 6));
	m_QUADVAO->SetIndexBuffer(quadEBO);
	#pragma endregion

	#pragma region SHADER
	auto flatColorShader = m_ShaderLib.Load("resource/shaders/flatColor.glsl");
	#pragma endregion
}

void SandBox2D::OnDetach() {}

void SandBox2D::OnUpdate(Kenshin::TimeStamp ts)
{
	m_CameraController.OnUpdate(ts);
	Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
	Kenshin::RendererCommand::Clear();
	Kenshin::Renderer::BeginScene(m_CameraController.GetCamera());
	auto flatColorShader = m_ShaderLib.Get("flatColor");
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
	flatColorShader->Bind();
	flatColorShader->SetVec3("u_Color", m_SquareColor);
	for (int x = -10; x < 10; x++)
	{
		for (int y = -10; y < 10; y++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0), pos) * scaleMatrix;
			Kenshin::Renderer::Submit(m_QUADVAO, flatColorShader, transform);
		}
	}
	Kenshin::Renderer::EndScene();
}

void SandBox2D::OnEvent(Kenshin::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("QuadColor");
	ImGui::ColorEdit3("QuadColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
