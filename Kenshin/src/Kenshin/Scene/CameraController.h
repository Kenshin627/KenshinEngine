#pragma once
#include "ScriptableEntity.h"

namespace Kenshin
{
	class CameraController : public ScriptableEntity
	{
	public:
		CameraController() = default;
		virtual~CameraController() = default;
		virtual void CameraController::OnCreate() override;
		virtual void CameraController::OnDestroy() override;
		virtual void CameraController::OnUpdate(TimeStamp ts) override;
	private:
	};
}