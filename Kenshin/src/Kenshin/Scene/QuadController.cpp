#include "kspch.h"
#include "QuadController.h"
#include "Components.h"

namespace Kenshin
{
	void QuadController::OnCreate()
	{
		auto tag = m_Entity.GetComponent<TagComponent>();
		KS_INFO("{0} entity has created!", tag.Tag);

		auto& transform = m_Entity.GetComponent<TransformComponent>().Transform;
		transform[3][0] = 4.0f;
	}

	void QuadController::OnDestroy()
	{

	}

	void QuadController::OnUpdate(TimeStamp ts)
	{
		m_time += ts;
		float moveSpeed = 2.0f;
		auto& transform = m_Entity.GetComponent<TransformComponent>().Transform;
		transform[3][0] = glm::cos(m_time * moveSpeed) * 4.0f;
		transform[3][1] = glm::sin(m_time * moveSpeed) * 4.0f;
	}
}