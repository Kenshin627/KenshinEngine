#include "kspch.h"
#include "EditLayer.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <gtc/type_ptr.hpp>

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
		m_EditorCamera = EditorCamera();

		//Entites
		m_ActiveScene = CreateRef<Scene>();

		//Panels
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		//gizmoButtons
		m_GizmoBtns[0] = Texture2D::Create("resource/textures/translate.png");
		m_GizmoBtns[1] = Texture2D::Create("resource/textures/rotate.png");
		m_GizmoBtns[2] = Texture2D::Create("resource/textures/scale.png");
	}

	void EditLayer::OnDetach() { }

	void EditLayer::OnUpdate(TimeStamp ts)
	{
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		if (FrameBufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_EditorCamera.OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStatistics();
		if (m_ViewportActive)
		{			
			m_EditorCamera.OnUpdate(ts);
		}

		m_Framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
		RendererCommand::Clear();

		m_ActiveScene->RenderScene(ts, m_EditorCamera);

		m_Framebuffer->Unbind();
	}

	void EditLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditLayer::OnKeyPressed, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditLayer::OnMouseEvent, std::placeholders::_1));
	}

	bool EditLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
		{
			return false;
		}

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::Q:
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = -1;				
			}
			break;
		
		case Key::W:
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
			break;
		case Key::E:
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			}
			break;
		case Key::R:
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;
		}
		return true;
	}


	bool EditLayer::OnMouseEvent(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void EditLayer::OnImGuiRender()
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
		//style.WindowMinSize.x = 370.0f;
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

		//hierarchyPanel
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportOffset = ImGui::GetWindowPos();
		ImVec2 minContentPos = ImGui::GetWindowContentRegionMin();
		ImVec2 maxContentPos = ImGui::GetWindowContentRegionMax();
		m_ViewportBounds[0] = glm::vec2(minContentPos.x + viewportOffset.x, minContentPos.y + viewportOffset.y);
		m_ViewportBounds[1] = glm::vec2(maxContentPos.x + viewportOffset.x, maxContentPos.y + viewportOffset.y);

		

		m_ViewportActive = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();		
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportActive);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		/*ImGui::SameLine();
		for (auto& tex : m_GizmoBtns)
		{			
			ImGui::ImageButton(tex->GetPath().c_str(), (void*)tex->GetRendererID(), { 18, 18 });
		}*/

		//guizmo
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		auto camera = m_EditorCamera;
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (selectedEntity && m_GizmoType != -1)
		{
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			const float snapVlaues[3] = { snapValue, snapValue, snapValue };
			auto& proj = m_EditorCamera.GetProjection();
			auto& view = m_EditorCamera.GetViewMatrix();
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			auto transform = transformComponent.GetTransform();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), ImGuizmo::OPERATION(m_GizmoType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap? snapVlaues : 0);
			if (ImGuizmo::IsUsing())
			{				
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
				transformComponent.Translation = translation;
				transformComponent.Rotation = glm::radians(rotation);
				transformComponent.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}
