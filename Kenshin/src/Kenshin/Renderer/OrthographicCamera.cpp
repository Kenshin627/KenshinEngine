#include "kspch.h"
#include "OrthographicCamera.h"

namespace Kenshin
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :m_ViewMatrix(glm::mat4(1.0)), m_ProjectionMatrix(glm::mat4(1.0)), m_ViewProjectionMatrix(glm::mat4(1.0)), m_Position(glm::vec3(0.0)), m_Rotation(0.0f)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		if (m_Position != position)
		{
			m_Position = position;
			calcutelaViewProjectionMatrix();
		}
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		if (m_Rotation != rotation)
		{
			m_Rotation = rotation;
			calcutelaViewProjectionMatrix();
		}
	}

	void OrthographicCamera::calcutelaViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}