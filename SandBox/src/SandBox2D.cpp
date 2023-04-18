#include "kspch.h"
#include "SandBox2D.h"
#include <imgui.h>

SandBox2D::SandBox2D() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D"), m_SquareColor({ 0.2, 0.3, 0.8, 1.0f }) {}

void SandBox2D::OnAttach() 
{
	m_checkboardTexture = Kenshin::Texture2D::Create("resource/textures/Checkerboard.png");
}
void SandBox2D::OnDetach() { }

void SandBox2D::OnUpdate(Kenshin::TimeStamp ts)
{
	m_CameraController.OnUpdate(ts);

	Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
	Kenshin::RendererCommand::Clear();

	Kenshin::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Kenshin::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::vec4(0.2, 0.6, 0.5, 1.0));
	Kenshin::Renderer2D::DrawQuad(glm::vec2(-0.3f), glm::vec2(0.5f, 0.8), glm::vec4(0.8, 0.3, 0.7, 1.0));
		
	Kenshin::Renderer2D::EndScene();
}

void SandBox2D::OnEvent(Kenshin::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("QuadColor");
	ImGui::ColorEdit4("QuadColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
