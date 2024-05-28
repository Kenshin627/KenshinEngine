#pragma once
#include "Core.h"
#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"
#include "events/Event.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

namespace Kenshin {
	class KS_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);
		~OrthographicCameraController();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		bool OnMouseScrolled(MouseScrollEvent& e);
		bool OnWindwResize(WindowResizeEvent& e);
		Ref<OrthographicCamera> GetCamera() { return m_Camera; }
		const Ref<OrthographicCamera>& GetCamera() const { return m_Camera; }
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		Ref<OrthographicCamera> m_Camera;
		glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;;
		float m_cameraTranslationSpeed = 1.0f;
		float m_cameraRotationSpeed = 5.0f;
		float m_cameraZoomSpeed = 0.25;
		bool m_EnableRotation = false;
	};
}