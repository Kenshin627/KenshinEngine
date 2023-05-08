#pragma once
#include "Kenshin.h"
#include "panel/SceneHierarchyPanel .h"
#include "panel/ContentBrowserPanel.h"
#include "Kenshin/Scene/EditorCamera.h"

namespace Kenshin
{
	class EditLayer : public Layer
	{
	public:
		EditLayer();
		virtual ~EditLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStamp ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseEvent(MouseButtonPressedEvent& e);
		void UIToolBar();
		void OnSceneRuntimeStart();
		void OnSceneRuntimeStop();
		void OnSceneSimulationStart();
		void OnSceneSimulationStop();
	public:
		struct ColliderDebugger
		{
			bool Enable = true;
			glm::vec4& Color = glm::vec4(0.164, 0.721, 1.0, 1.0);
		};
	private:
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& p);
		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);
		void DuplicateEntity();
		void OnOverLayRender();
	private:
		enum class SceneStats { Editor = 0, Play, Simulation };
		OrthoGraphicCameraController m_CameraController;
		EditorCamera m_EditorCamera;
		Ref<FrameBuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportHovered = false;
		bool m_ViewportFocus = false;
		Entity m_HoveredEntity;
		Ref<Scene> m_EditScene;
		Ref<Scene> m_ActiveScene;
		std::filesystem::path m_EditScenePath = std::filesystem::path();
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		Ref<Texture2D> m_PlayIcon;
		Ref<Texture2D> m_StopIcon;
		Ref<Texture2D> m_SimulationIcon;
		int m_GizmoType = -1;
		SceneStats m_SceneStats = SceneStats::Editor;

		ColliderDebugger collider;
	};
}