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
		auto& transform = GetComponent<TransformComponent>();
		float moveSpeed = 10.0f;
		KS_CORE_INFO("{0} (ms)", ts.GetmileSeconds());
		glm::vec3 translateOffset;
		bool isMoved = false;
		if (Input::IsKeyPressed(Key::W))
		{
			isMoved = true;
			translateOffset = { 0.0f, moveSpeed * ts, 0.0f };			
		}

		if (Input::IsKeyPressed(Key::S))
		{
			isMoved = true;
			translateOffset = { 0.0f, -moveSpeed * ts, 0.0f };
		}

		if (Input::IsKeyPressed(Key::A))
		{
			isMoved = true;
			translateOffset = { -moveSpeed * ts, 0.0f, 0.0f };
		}

		if (Input::IsKeyPressed(Key::D))
		{
			isMoved = true;
			translateOffset = { moveSpeed * ts, 0.0f, 0.0f };
		}

		if (isMoved)
		{
			transform.Transform = glm::translate(glm::mat4(1.0f), translateOffset) * transform.Transform;
		}
	}

	void CameraController::OnDestroy()
	{

	}
}