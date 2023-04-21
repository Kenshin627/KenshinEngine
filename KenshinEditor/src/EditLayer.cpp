#include "kspch.h"
#include "EditLayer.h"
#include <imgui.h>

namespace Kenshin
{
	EditLayer::EditLayer() : m_CameraController(1280.0f / 720.0f), Layer("SandBox2D"), m_SquareColor({ 0.2, 0.3, 0.8, 1.0f }) {}

	void EditLayer::OnAttach()
	{
		m_Framebuffer = FrameBuffer::Create(FrameBufferSpecification(1280, 720, 1, false));
		m_checkboardTexture = Texture2D::Create("resource/textures/Checkerboard.png");
		m_BandTexture = Texture2D::Create("resource/textures/ledZepppelin.jpg");
		m_SpiriteSheet = Texture2D::Create("resource/textures/RPGpack_sheet.png");
		m_SpirteAnima = Texture2D::Create("resource/textures/square_nodetailsOutline.png");
		m_Tree = SubTexture2D::Create(m_SpiriteSheet, { 0, 1 }, { 64, 64 }, { 1.0f, 2.0f });
		m_Pig = SubTexture2D::Create(m_SpirteAnima, { 4, 4 }, { 136, 136 });
		m_Cat = SubTexture2D::Create(m_SpirteAnima, { 0, 3 }, { 136, 136 });
		m_CameraController.SetZoomLevel(5.5f);
	}

	void EditLayer::OnDetach() { }

	void EditLayer::OnUpdate(TimeStamp ts)
	{
		if (FrameBufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStatistics();
		if (m_ViewportActive)
		{
			m_CameraController.OnUpdate(ts);
		}

		m_Framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
		RendererCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
#if 0
		Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::vec4(0.2, 0.6, 0.5, 1.0));
		Renderer2D::DrawQuad(glm::vec2(-0.3f), glm::vec2(0.5f, 0.8), glm::vec4(0.8, 0.3, 0.7, 1.0));

		Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(5.0f), m_checkboardTexture, 10.0f);
		Renderer2D::DrawRotateQuad(glm::vec2(1.0f, 0.0f), rotation, glm::vec2(1.0f), m_BandTexture, 1.0f);

#endif // 0		
		Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f, 2.0f), m_Tree);
		Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Pig);
		Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_Cat);
		Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

	void EditLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditLayer::OnImGuiRender(TimeStamp ts)
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		ImGui::Begin("Stats");
		auto stats = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportActive = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();		
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportActive);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}
