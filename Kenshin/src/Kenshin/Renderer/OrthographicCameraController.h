#pragma once
#include "OrthographicCamera.h"
#include "Kenshin/Core/TimeStamp.h"
#include "Kenshin/Events/Event.h"
#include "Kenshin/Events/ApplicationEvent.h"
#include "Kenshin/Events/MouseEvent.h"

namespace Kenshin
{
	class  OrthoGraphicCameraController
	{
	public:
		OrthoGraphicCameraController(float aspectRatio, bool rotaion = false, float zoomSpeed = 0.3f, const glm::vec2& zoomlevelBounds = { 0.1f, 2.0f });
		~OrthoGraphicCameraController() = default;
		void OnUpdate(TimeStamp ts);
		void OnEvent(Event& e);
		void OnResize(uint32_t width, uint32_t height);
		void SetZoomLevel(float zoomLevel);
		float GetZoomLevel() const { return m_ZoomLevel; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		bool m_EnableRotation;

		glm::vec3 m_CameraPosition;
		float m_CameraRotation;
		float m_CameraTranslateSpeed;
		float m_CameraRotationSpeed;
		float m_ZoomSpeed;
		glm::vec2 m_ZoomlevelBounds; //x:min y:max

		OrthographicCamera m_Camera;
	};
}