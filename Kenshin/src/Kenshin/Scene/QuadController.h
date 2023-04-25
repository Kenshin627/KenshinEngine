#pragma once
#include "ScriptableEntity.h"
#include "Kenshin/Core/Core.h"

namespace Kenshin
{
	class KENSHIN_API QuadController : public ScriptableEntity
	{
	public:
		QuadController() = default;
		virtual~QuadController() = default;
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate(TimeStamp ts) override;
	private:
		float m_time{ 0.0f };
	};
}