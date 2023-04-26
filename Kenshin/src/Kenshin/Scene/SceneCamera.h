#pragma once
#include "Kenshin/Renderer/Camera.h"

namespace Kenshin
{
	enum class ProjectionType { Perspective = 0, Orthographic };
	class  SceneCamera : public Camera
	{
	public:
		SceneCamera();

		virtual~SceneCamera() = default;
		
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFov, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		float GetPerpectiveVerticalFov() const { return m_PerpectiveVerticalFOV; }
		void SetPerpectiveVerticalFov(float fov) { m_PerpectiveVerticalFOV = fov; RecalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerpectiveNear; }
		void SetPerspectiveNear(float nearClip) { m_PerpectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerpectiveFar; }
		void SetPerspectiveFar(float farClip) { m_PerpectiveFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
		float GetAspectRatio() const { return m_AspectRatio; }
		void RecalculateProjection();
	private:
		//common
		float m_AspectRatio{ 0.0f };
		ProjectionType m_ProjectionType{ ProjectionType::Orthographic };

		//Orhtographic
		float m_OrthographicSize{ 10.0f };
		float m_OrthographicNear{ -1.0f };
		float m_OrthographicFar{ 1.0f };

		//Perspective
		float m_PerpectiveVerticalFOV{ glm::radians(45.0f) };
		float m_PerpectiveNear{ 0.01f };
		float m_PerpectiveFar{ 100.0f };
	};
}