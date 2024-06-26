#include "kspch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include <GLFW/glfw3.h>

namespace Kenshin {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotation) :
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1.0f),
		m_Camera(std::make_shared<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)),
		m_EnableRotation(enableRotation) {}

	OrthographicCameraController::~OrthographicCameraController() {}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition.x -= m_cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition.x += m_cameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_CameraPosition.y -= m_cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition.y += m_cameraTranslationSpeed * ts;
		}

		if (m_EnableRotation)
		{
			if (Input::IsKeyPressed(GLFW_KEY_Q))
			{
				m_CameraRotation -= m_cameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(GLFW_KEY_E))
			{
				m_CameraRotation += m_cameraRotationSpeed * ts;
			}
			m_Camera->SetRotation(m_CameraRotation);
		}

		m_Camera->SetPosition(m_CameraPosition);		
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(BIND_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_FN(OrthographicCameraController::OnWindwResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY() * m_cameraZoomSpeed;
		m_ZoomLevel = std::max(0.25f, m_ZoomLevel);
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindwResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}