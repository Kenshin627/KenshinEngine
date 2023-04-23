#pragma once
#include <glm.hpp>

namespace Kenshin
{
	class Camera
	{
	public:
		Camera() = default;
		virtual~Camera() = default;
		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection{ 1.0f };
	};
}