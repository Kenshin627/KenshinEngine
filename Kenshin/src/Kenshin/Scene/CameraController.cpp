#include "kspch.h"
#include "CameraController.h"
#include "Kenshin/Core/Input.h"
#include "Kenshin/Scene/Components.h"

namespace Kenshin
{
	void CameraController::OnCreate() 
	{
		KS_CORE_INFO("cameraController::OnCreate");
	}

	void CameraController::OnUpdate(TimeStamp ts)
	{
		auto& transformComponent = GetComponent<TransformComponent>();
		float moveSpeed = 10.0f;
		if (Input::IsKeyPressed(Key::W))
		{
			transformComponent.Translation += glm::vec3{ 0.0f, moveSpeed * ts, 0.0f };
		}

		if (Input::IsKeyPressed(Key::S))
		{
			transformComponent.Translation += glm::vec3{ 0.0f, -moveSpeed * ts, 0.0f };
		}

		if (Input::IsKeyPressed(Key::A))
		{
			transformComponent.Translation += glm::vec3{ -moveSpeed * ts, 0.0f, 0.0f };
		}

		if (Input::IsKeyPressed(Key::D))
		{
			transformComponent.Translation += glm::vec3{ moveSpeed * ts, 0.0f, 0.0f };
		}
	}

	void CameraController::OnDestroy()
	{

	}
}