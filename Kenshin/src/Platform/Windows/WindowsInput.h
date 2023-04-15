#pragma once
#include "Kenshin/Input.h"

namespace Kenshin
{
	class WindowsInput:public Input
	{
	public:
		WindowsInput();
		~WindowsInput();
		virtual bool IsKeyPressedImpl(KeyCode key) override;
		virtual bool IsMousePressedImpl(MouseCode button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}