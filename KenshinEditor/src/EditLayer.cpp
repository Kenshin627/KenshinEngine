#include "kspch.h"
#include "EditLayer.h"
#include "Kenshin/Math/Math.h"
#include "Kenshin/Scene/SceneSerializer.h"
#include "Kenshin/Utils/PlatformUtils.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <gtc/type_ptr.hpp>
#include <entt.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/matrix_decompose.hpp>

namespace Kenshin
{
	EditLayer::EditLayer() : m_CameraController(1280.0f / 720.0f), Layer("KenshinEditor") {}

	void EditLayer::OnAttach()
	{
		FrameBufferSpecification fbSpec;
		fbSpec.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Samples = 1;
		fbSpec.SwapChainTarget = false;
		m_Framebuffer = FrameBuffer::Create(fbSpec);
		
		m_CameraController.SetZoomLevel(5.5f);
		m_EditorCamera = EditorCamera();

		//Entites
		m_EditScene = CreateRef<Scene>();
		m_ActiveScene = m_EditScene;
		//Panels
		m_SceneHierarchyPanel.SetContext(m_EditScene);

		m_PlayIcon = Texture2D::Create("resource/toolBar/PlayButton.png");
		m_StopIcon = Texture2D::Create("resource/toolBar/StopButton.png");
		m_SimulationIcon = Texture2D::Create("resource/toolBar/SimulateButton.png");

		Renderer2D::SetLineWidth(3.0f);
	}

	void EditLayer::OnDetach() { }

	void EditLayer::OnUpdate(TimeStamp ts)
	{
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		if (FrameBufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_EditorCamera.OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			/*m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);*/
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStatistics();		
		m_Framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4{ 0.2, 0.2, 0.2, 1.0 });
		RendererCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);

		switch (m_SceneStats)
		{
		case Kenshin::EditLayer::SceneStats::Editor:
			if (m_ViewportFocus && m_ViewportHovered)
			{
				m_EditorCamera.OnUpdate(ts);
			}
			m_ActiveScene->OnUpdateEditor(m_EditorCamera);
			break;
		case Kenshin::EditLayer::SceneStats::Play:
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		case Kenshin::EditLayer::SceneStats::Simulation:			
			m_EditorCamera.OnUpdate(ts);			
			m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}
		
		if (collider.Enable && m_ActiveScene->GetPrimaryCamera())
		{
			OnOverLayRender();
		}

		auto [mpx, mpy] = ImGui::GetMousePos();
		mpx -= m_ViewportBounds[0].x;
		mpy -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		mpy = viewportSize.y - mpy;
		int mouseX = (int)mpx;
		int mouseY = (int)mpy;

		if (mouseX >= 0 && mouseX < (int)viewportSize.x && mouseY >= 0 && mouseY < (int)viewportSize.y)
		{
			int data = m_Framebuffer->ReadPixel(1, mouseX, mouseY);			
			m_HoveredEntity = data == -1? Entity() : Entity((entt::entity)data, m_ActiveScene.get());			
		}

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
		//files
		case Key::O:
			if (control)
			{
				OpenScene();				
			}
			break;
		case Key::N:
			if (control)
			{
				NewScene();				
			}
			break;
		case Key::S:
			if (control)
			{
				if (shift)
				{
					SaveSceneAs();
				}
				else {
					SaveScene();
				}
			}
			break;

		//ImGuizmo
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
		case Key::D:
			if (control)
			{
				DuplicateEntity();
			}
			break;
		}		
		return true;
	}

	bool EditLayer::OnMouseEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft && m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
		{
			m_SceneHierarchyPanel.SetSelectiedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditLayer::OnImGuiRender()
	{				
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_AutoHideTabBar;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{				
				if (ImGui::MenuItem("Open", "Ctrl + O"))
				{
					OpenScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("New", "Ctrl + N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Save As", "Ctrl + Shift + S"))
				{
					SaveSceneAs();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Ctrl + E"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//hierarchyPanel
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();		

		auto selectionEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		ImGui::Begin("Stats");
		auto stats = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		const char* selectedTag = selectionEntity ? selectionEntity.GetComponent<TagComponent>().Tag.c_str() : "NONE";
		ImGui::Text("Selected Entity: %s", selectedTag);
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Visualize Collider", &collider.Enable);
		ImGui::ColorEdit4("Collider Color", glm::value_ptr(collider.Color));
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportOffset = ImGui::GetWindowPos();
		ImVec2 minContentPos = ImGui::GetWindowContentRegionMin();
		ImVec2 maxContentPos = ImGui::GetWindowContentRegionMax();
		m_ViewportBounds[0] = glm::vec2(minContentPos.x + viewportOffset.x, minContentPos.y + viewportOffset.y);
		m_ViewportBounds[1] = glm::vec2(maxContentPos.x + viewportOffset.x, maxContentPos.y + viewportOffset.y);

		
		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
			
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* itemPath = (const wchar_t*)payload->Data;
				OpenScene(itemPath);
				ImGui::EndDragDropTarget();
			}
		}

		//guizmo
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		if (selectionEntity && m_GizmoType != -1 && m_SceneStats == SceneStats::Editor)
		{
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			const float snapVlaues[3] = { snapValue, snapValue, snapValue };
			auto& proj = m_EditorCamera.GetProjection();
			glm::mat4 view = m_EditorCamera.GetViewMatrix();
			auto& transformComponent = selectionEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), ImGuizmo::OPERATION(m_GizmoType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap? snapVlaues : nullptr);
			if (ImGuizmo::IsUsing())
			{				
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				auto deltaRotation = rotation - transformComponent.Rotation;
				transformComponent.Translation = translation;
				transformComponent.Rotation += deltaRotation;
				transformComponent.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UIToolBar();

		ImGui::End();
	}

	void EditLayer::UIToolBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		auto& buttonActive = colors[ImGuiCol_ButtonActive];
		auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::Begin("##UITool", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float height = ImGui::GetWindowHeight();
		float size = height - 4.0f;
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);
		auto playIcon = m_SceneStats != SceneStats::Play ? m_PlayIcon : m_StopIcon;
		if (ImGui::ImageButton((ImTextureID)playIcon->GetRendererID(), { size, size }, { 0,1 }, {1,0}, 0))
		{
			if (m_SceneStats != SceneStats::Play)
			{
				OnSceneRuntimeStart();				
			}
			else
			{
				OnSceneRuntimeStop();				
			}
		}

		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x * 0.5f + size * 0.5f);
		auto simulationIcon = m_SceneStats != SceneStats::Simulation ? m_SimulationIcon : m_StopIcon;
		if (ImGui::ImageButton((ImTextureID)simulationIcon->GetRendererID(), { size, size }, { 0, 1 }, { 1, 0 }, 0))
		{
			if(m_SceneStats != SceneStats::Simulation)
			{
				OnSceneSimulationStart();
			}
			else
			{
				OnSceneSimulationStop();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditLayer::OnSceneRuntimeStart()
	{
		if (m_SceneStats == SceneStats::Simulation)
		{
			OnSceneSimulationStop();
		}
		m_ActiveScene = Scene::Copy(m_EditScene);		
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnRuntimeStart();
		m_SceneStats = SceneStats::Play;
	}

	void EditLayer::OnSceneRuntimeStop()
	{
		m_ActiveScene->OnRuntimeStop();
		m_SceneStats = SceneStats::Editor;
		m_ActiveScene = m_EditScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditLayer::OnSceneSimulationStart()
	{
		if (m_SceneStats == SceneStats::Play)
		{
			OnSceneRuntimeStop();
		}
		m_ActiveScene = Scene::Copy(m_EditScene);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnSimulationStart();
		m_SceneStats = SceneStats::Simulation;
	}

	void EditLayer::OnSceneSimulationStop()
	{
		m_ActiveScene->OnSimulationStop();
		m_SceneStats = SceneStats::Editor;
		m_ActiveScene = m_EditScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditLayer::NewScene()
	{
		m_EditScene = CreateRef<Scene>();
		m_ActiveScene = m_EditScene;
		m_EditScenePath = std::filesystem::path();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditLayer::OpenScene()
	{
		auto path = FileDialog::OpenFile("Kenshin Scene(*.kenshin)\0*.kenshin\0");
		OpenScene(path);
	}

	void EditLayer::OpenScene(const std::filesystem::path& p)
	{
		if (m_SceneStats == SceneStats::Play)
		{
			OnSceneRuntimeStop();
		}

		if (m_SceneStats == SceneStats::Simulation)
		{
			OnSceneSimulationStop();
		}

		if (p.extension().string() != ".kenshin")
		{
			KS_CORE_WARN("Could not load {0} - not a scene file", p.filename().string());
			return;
		}
		if (!p.empty())
		{
			Ref<Scene> newScene = CreateRef<Scene>();
			SceneSerializer ss(newScene);
			ss.DeSerialize(p.string());
			m_EditScenePath = p.string();
			m_EditScene = newScene;
			m_ActiveScene = newScene;
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}
	}
	
	void EditLayer::SaveScene()
	{
		if (!m_EditScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}	

	void EditLayer::SaveSceneAs() 
	{
		auto path = FileDialog::SaveFile("Kenshin Scene (*.kenshin)\0*.kenshin\0");
		if (!path.empty())
		{
			SerializeScene(m_ActiveScene, path);
			m_EditScenePath = path;
		}
	}

	void EditLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer ss(m_ActiveScene);
		ss.Serialize(path.string());
	}

	void EditLayer::DuplicateEntity()
	{
		if (m_SceneStats != SceneStats::Editor)
		{
			return;
		}
		auto selectEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectEntity)
		{
			Entity newEntity = m_ActiveScene->DuplicateEntity(selectEntity);
			m_SceneHierarchyPanel.SetSelectiedEntity(newEntity);
		}
	}

	void EditLayer::OnOverLayRender()
	{
		if (m_SceneStats == SceneStats::Play)
		{
			auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
			auto camera = cameraEntity.GetComponent<CameraComponent>();
			auto transform = cameraEntity.GetComponent<TransformComponent>();
			Renderer2D::BeginScene(camera.Camera, transform.GetTransform());
		}
		else 
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		//circle Collider
		auto circleColliders = m_ActiveScene->GetAllEntitiesWith<CircleCollider2DComponent, TransformComponent>();
		for (auto& entity : circleColliders)
		{
			auto& [cc2d, tc] = circleColliders.get<CircleCollider2DComponent, TransformComponent>(entity);
			if (cc2d.Visualize)
			{
				glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.01f);
				glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0), translation) * glm::scale(glm::mat4(1.0), scale);
				Renderer2D::DrawCircle(transform, collider.Color, 0.03, 0.005, -1);
			}
		}

		//box Collider
		auto boxColliders = m_ActiveScene->GetAllEntitiesWith<BoxCollider2DComponent, TransformComponent>();
		for (auto& entity : boxColliders)
		{
			auto& [bc2d, tc] = boxColliders.get<BoxCollider2DComponent, TransformComponent>(entity);
			if (bc2d.Visualize)
			{
				glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.01f);
				glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0), scale);
				Renderer2D::DrawRect(transform, collider.Color, -1);
			}
		}
		Renderer2D::EndScene();
	}
}
