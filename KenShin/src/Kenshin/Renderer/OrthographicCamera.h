#pragma once
#include <glm/glm.hpp>

namespace Kenshin {
	class OrthographicCamera
	{
	public:
		OrthographicCamera() {}
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
		void SetPosition(const glm::vec3& pos);
		void SetRotation(float rotation);
		inline const glm::vec3& GetPostion() const { return m_Position; }
		inline const float GetRotation() const { return m_Rotation; }
		inline const glm::mat4& ViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& ProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& ViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void ReCalcViewMatrix();
	private:
		glm::vec3 m_Position { 0.0f, 0.0f, 0.0f };
		float     m_Rotation = 0.0f;
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	};
}