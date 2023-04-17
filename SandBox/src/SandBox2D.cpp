#include "kspch.h"
#include "SandBox2D.h"
#include <imgui.h>

SandBox2D::SandBox2D() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D"), m_SquareColor({ 0.2, 0.3, 0.8 }) {}

void SandBox2D::OnAttach() 
{
	Kenshin::Renderer2D::Init();
}

void SandBox2D::OnDetach() 
{
	Kenshin::Renderer2D::ShutDown();
}

void SandBox2D::OnUpdate(Kenshin::TimeStamp ts)
{
	m_CameraController.OnUpdate(ts);

	Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
	Kenshin::RendererCommand::Clear();

	Kenshin::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (int x = -10; x < 10; x++)
	{
		for (int y = -10; y < 10; y++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			Kenshin::Renderer2D::DrawQuad(pos, { 0.1, 0.1 }, m_SquareColor);
		}
	}
	Kenshin::Renderer2D::EndScene();
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
