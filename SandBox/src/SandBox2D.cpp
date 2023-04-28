#include "kspch.h"
#include "SandBox2D.h"
#include <imgui.h>

SandBox2D::SandBox2D() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D") {}

void SandBox2D::OnAttach()
{
	m_checkboardTexture = Kenshin::Texture2D::Create("resource/textures/Checkerboard.png");
	m_BandTexture = Kenshin::Texture2D::Create("resource/textures/ledZepppelin.jpg");
	m_SpiriteSheet = Kenshin::Texture2D::Create("resource/textures/RPGpack_sheet.png");
	m_SpirteAnima = Kenshin::Texture2D::Create("resource/textures/square_nodetailsOutline.png");
	m_Tree = Kenshin::SubTexture2D::Create(m_SpiriteSheet, { 0, 1 }, { 64, 64 }, { 1.0f, 2.0f });
	m_Pig = Kenshin::SubTexture2D::Create(m_SpirteAnima, { 4, 4 }, { 136, 136 });
	m_Cat = Kenshin::SubTexture2D::Create(m_SpirteAnima, { 0, 3 }, { 136, 136 });
	m_CameraController.SetZoomLevel(5.5f);
}

void SandBox2D::OnDetach() { }

void SandBox2D::OnUpdate(Kenshin::TimeStamp ts)
{
	Kenshin::Renderer2D::ResetStatistics();
	m_CameraController.OnUpdate(ts);
	Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
	Kenshin::RendererCommand::Clear();
	Kenshin::Renderer2D::BeginScene(m_CameraController.GetCamera());
	/*Kenshin::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f, 2.0f), m_Tree);
	Kenshin::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Pig);
	Kenshin::Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Cat);*/
	Kenshin::Renderer2D::EndScene();
}

void SandBox2D::OnEvent(Kenshin::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImGuiRender()
{
	auto stattistics = Kenshin::Renderer2D::GetStatistics();
	ImGui::Begin("Statistics");	
	ImGui::Text("DrawCalls: %d", stattistics.DrawCalls);
	ImGui::Text("QuadCount: %d", stattistics.QuadCount);
	ImGui::Text("TotalVertices: %d", stattistics.GetTotalVertexCount());
	ImGui::Text("TotalIndices: %d", stattistics.GetTotalIndexCount());	
	ImGui::End();
}
