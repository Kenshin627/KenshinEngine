#include "kspch.h"
#include "EditLayer.h"
#include <imgui.h>

EditLayer::EditLayer() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D"), m_SquareColor({ 0.2, 0.3, 0.8, 1.0f }) {}

void EditLayer::OnAttach()
{
	m_Framebuffer = Kenshin::FrameBuffer::Create(Kenshin::FrameBufferSpecification(960, 520, 1, false));

	m_checkboardTexture = Kenshin::Texture2D::Create("resource/textures/Checkerboard.png");
	m_BandTexture = Kenshin::Texture2D::Create("resource/textures/ledZepppelin.jpg");
	m_SpiriteSheet = Kenshin::Texture2D::Create("resource/textures/RPGpack_sheet.png");
	m_SpirteAnima = Kenshin::Texture2D::Create("resource/textures/square_nodetailsOutline.png");
	m_Tree = Kenshin::SubTexture2D::Create(m_SpiriteSheet, { 0, 1 }, { 64, 64 }, { 1.0f, 2.0f });
	m_Pig = Kenshin::SubTexture2D::Create(m_SpirteAnima, { 4, 4 }, { 136, 136 });
	m_Cat = Kenshin::SubTexture2D::Create(m_SpirteAnima, { 0, 3 }, { 136, 136 });
	m_CameraController.SetZoomLevel(5.5f);
}

void EditLayer::OnDetach() { }

void EditLayer::OnUpdate(Kenshin::TimeStamp ts)
{
	Kenshin::Renderer2D::ResetStatistics();
	m_CameraController.OnUpdate(ts);

	m_Framebuffer->Bind();

	Kenshin::RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
	Kenshin::RendererCommand::Clear();

	Kenshin::Renderer2D::BeginScene(m_CameraController.GetCamera());
	#if 0
	Kenshin::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::vec4(0.2, 0.6, 0.5, 1.0));
	Kenshin::Renderer2D::DrawQuad(glm::vec2(-0.3f), glm::vec2(0.5f, 0.8), glm::vec4(0.8, 0.3, 0.7, 1.0));

	Kenshin::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(5.0f), m_checkboardTexture, 10.0f);
	Kenshin::Renderer2D::DrawRotateQuad(glm::vec2(1.0f, 0.0f), rotation, glm::vec2(1.0f), m_BandTexture, 1.0f);

	#endif // 0		
	Kenshin::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f, 2.0f), m_Tree);
	Kenshin::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Pig);
	Kenshin::Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Cat);
	Kenshin::Renderer2D::EndScene();

	m_Framebuffer->Unbind();
}

void EditLayer::OnEvent(Kenshin::Event& e)
{
	m_CameraController.OnEvent(e);
}

void EditLayer::OnImGuiRender(Kenshin::TimeStamp ts)
{
	auto stattistics = Kenshin::Renderer2D::GetStatistics();
	ImGui::Begin("Statistics");	
	ImGui::Text("DrawCalls: %d", stattistics.DrawCalls);
	ImGui::Text("QuadCount: %d", stattistics.QuadCount);
	ImGui::Text("TotalVertices: %d", stattistics.GetTotalVertexCount());
	ImGui::Text("TotalIndices: %d", stattistics.GetTotalIndexCount());
	
	ImGui::Text("TimeStamp: %d ms", ts.GetmileSeconds());
	ImGui::End();

	ImGui::Begin("viewport");
	ImGui::Image((void*)m_Framebuffer->GetColorAttachment(), {960, 460}, {0,1}, {1,0});
	ImGui::End();
}
