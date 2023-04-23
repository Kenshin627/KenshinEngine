#pragma once
#include "Kenshin/Renderer/Camera.h"

namespace Kenshin
{
	class KENSHIN_API SceneCamera : public Camera
	{
	public:
		SceneCamera();

		virtual~SceneCamera() = default;
		
		void SetOrthographic(float size, float near, float far);

		float GetViewportSize() const { return m_OrthographicSize; }
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetNearClip() const { return m_OrthographicNear; }
		void SetNearClip(float nearClip);

		float GetFarClip() const { return m_OrthographicFar; }
		void SetFarClip(float farClip);

		float GetAspectRatio() const { return m_AspectRatio; }

		void RecalculateProjection();
	private:
		float m_AspectRatio{ 0.0f };
		float m_OrthographicSize{ 10.0f };
		float m_OrthographicNear{ -1.0f };
		float m_OrthographicFar{ 1.0f };
	};
}