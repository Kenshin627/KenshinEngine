#include "kspch.h"
#include "QuadController.h"
#include "Components.h"

namespace Kenshin
{
	void QuadController::OnCreate()
	{
		auto tag = m_Entity.GetComponent<TagComponent>();
		KS_INFO("{0} entity has created!", tag.Tag);

		auto& transformComponent = m_Entity.GetComponent<TransformComponent>();
		transformComponent.Translation.x = 4.0f;
	}

	void QuadController::OnDestroy()
	{

	}

	void QuadController::OnUpdate(TimeStamp ts)
	{
		m_time += ts;
		float moveSpeed = 2.0f;
		auto& transformComponent = m_Entity.GetComponent<TransformComponent>();
		transformComponent.Translation.x = glm::cos(m_time * moveSpeed) * 4.0f;
		transformComponent.Translation.y = glm::sin(m_time * moveSpeed) * 4.0f;
	}
}