#include "kspch.h"
#include "SceneCamera.h"

namespace Kenshin
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerpectiveVerticalFOV = verticalFov;
		m_PerpectiveNear = nearClip;
		m_PerpectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		KS_CORE_ASSERT(width > 0 && height > 0, "invalid viewportsize!");
		float ratio = (float)width / (float)height;
		if (m_AspectRatio != ratio)
		{
			m_AspectRatio = ratio;
			RecalculateProjection();
		}
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float left = -m_AspectRatio * m_OrthographicSize * 0.5f;
			float right = m_AspectRatio * m_OrthographicSize * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;
			float top = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}
		else if(m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerpectiveVerticalFOV, m_AspectRatio, m_PerpectiveNear, m_PerpectiveFar);
		}
	}
}