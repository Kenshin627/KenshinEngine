#pragma once
#include "Core.h"
#include <utility>

namespace Kenshin {
	class KS_API Input
	{
	public:
		static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		static bool IsMouseButtonPressed(int button) { return s_Instance->IsKeyPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMousePosX() { return s_Instance->GetMousePosXImpl(); }
		static float GetMousePosY() { return s_Instance->GetMousePosYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) const = 0;
		virtual bool IsMouseButtonPressedImpl(int button) const = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const = 0;
		virtual float GetMousePosXImpl() const = 0;
		virtual float GetMousePosYImpl() const = 0;
	private:
		static Input* s_Instance;
	};
}