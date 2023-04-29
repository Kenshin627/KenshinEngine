#pragma once
#include "Kenshin.h"
#include "panel/SceneHierarchyPanel .h"
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
	private:
		OrthoGraphicCameraController m_CameraController;
		EditorCamera m_EditorCamera;
		glm::vec4 m_SquareColor;
		Ref<Texture2D> m_checkboardTexture;
		Ref<Texture2D> m_BandTexture;
		Ref<Texture2D> m_SpiriteSheet;
		Ref<Texture2D> m_SpirteAnima;
		Ref<SubTexture2D> m_Tree;
		Ref<SubTexture2D> m_Bush;
		Ref<SubTexture2D> m_Pig;
		Ref<SubTexture2D> m_Cat;
		Ref<FrameBuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportHovered = false;
		bool m_ViewportFocus = false;
		Entity m_HoveredEntity;
		Entity m_SelectedEntity;
		Ref<Scene> m_ActiveScene;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Ref<Texture2D> m_GizmoBtns[3];
		int m_GizmoType = -1;
	};
}