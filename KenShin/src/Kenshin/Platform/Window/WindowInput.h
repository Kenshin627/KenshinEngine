#pragma once

#include "Kenshin/Input.h"

namespace Kenshin {
	class WindowInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) const override;
		virtual bool IsMouseButtonPressedImpl(int button) const override;
		virtual std::pair<float, float> GetMousePositionImpl() const override;
		virtual float GetMousePosXImpl() const override;
		virtual float GetMousePosYImpl() const override;
	};
}