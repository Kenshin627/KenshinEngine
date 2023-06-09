#pragma once
#include "glm.hpp"

namespace Kenshin
{
	class  OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float translateSpeed = 5.0f, float rotationSpeed = 180.0f);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position);

		const glm::vec3& GetPosition() const { return m_Position; }
		
		void SetRotation(float rotation);

		float GetRotation() const { return m_Rotation; }

		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		float GetTranslateSpeed() const { return m_TranslateSpeed; }

		float GetRotationSpeed() const { return m_RotationSpeed; }

	private:
		void calcutelaViewProjectionMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
		float m_Rotation;
		float m_TranslateSpeed;
		float m_RotationSpeed;
	};
}