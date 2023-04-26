#pragma once
#include "Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Kenshin
{
	class  Input
	{
	public:
		Input() {};
		virtual~Input() {};
		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); };
		inline static bool IsMousePressed(MouseCode button) { return s_Instance->IsMousePressedImpl(button); };
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); };
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); };
	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;
		virtual bool IsMousePressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};
}