#pragma once
#include "Core.h"

namespace Kenshin
{
	class KENSHIN_API Input
	{
	public:
		Input() {};
		virtual~Input() {};
		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); };
		inline static bool IsMousePressed(int button) { return s_Instance->IsMousePressedImpl(button); };
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); };
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); };
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMousePressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};
}