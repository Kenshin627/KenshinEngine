#include "kspch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Kenshin {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ViewMatrix(glm::mat4(1.0f)),
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top)),
		m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	
	void OrthographicCamera::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		ReCalcViewMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		ReCalcViewMatrix();
	}

	void OrthographicCamera::ReCalcViewMatrix()
	{
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}