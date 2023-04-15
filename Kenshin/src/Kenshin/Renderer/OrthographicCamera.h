#pragma once
#include "glm.hpp"

namespace Kenshin
{
	class KENSHIN_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position);

		const glm::vec3& GetPosition() const { return m_Position; }
		
		void SetRotation(float rotation);

		float GetRotation() const { return m_Rotation; };

		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
	
	private:
		void calcutelaViewProjectionMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
		float m_Rotation;
	};
}