#include "kspch.h"
#include "OrthographicCameraController.h"
#include "Kenshin/Core/Input.h"

namespace Kenshin
{
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotation, float zoomSpeed, const glm::vec2& zoomlevelBounds) :
		m_AspectRatio(aspectRatio), 
		m_ZoomLevel(1.0f), 
		m_EnableRotation(rotation), 
		m_CameraPosition({ 0.0f, 0.0f, 0.0f }), 
		m_CameraRotation(0.0f), 
		m_CameraTranslateSpeed(5.0f), 
		m_CameraRotationSpeed(180.0f),
		m_ZoomSpeed(zoomSpeed),
		m_ZoomlevelBounds(zoomlevelBounds),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_CameraTranslateSpeed, m_CameraRotationSpeed) {}

	void OrthoGraphicCameraController::OnUpdate(TimeStamp ts)
	{
		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslateSpeed * ts;
		}

		if (m_EnableRotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslateSpeed = m_ZoomLevel;
	}

	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthoGraphicCameraController::OnWindowResize, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthoGraphicCameraController::OnMouseScrolled, std::placeholders::_1));
	}

	bool OrthoGraphicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		unsigned width =  e.GetWidth();
		unsigned height = e.GetHeight();
		OnResize(width, height);
		return false;
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * m_ZoomSpeed;
		m_ZoomLevel = glm::min(glm::max(m_ZoomLevel, m_ZoomlevelBounds.x), m_ZoomlevelBounds.y);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	void OrthoGraphicCameraController::OnResize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthoGraphicCameraController::SetZoomLevel(float level)
	{
		m_ZoomLevel = level;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
}