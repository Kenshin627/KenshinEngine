#pragma once
#include <glm.hpp>

namespace Kenshin
{
	class Camera
	{
	public:
		Camera() = default;
		const glm::mat4& GetProjection() const { return m_Projection; }
	private:
		glm::mat4 m_Projection;
	};
}