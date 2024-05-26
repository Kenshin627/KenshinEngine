#include "kspch.h"
#include "WindowInput.h"
#include "Kenshin/Application.h"
#include <GLFW/glfw3.h>

namespace Kenshin {
	Input* Input::s_Instance = new WindowInput();
	bool WindowInput::IsKeyPressedImpl(int keyCode) const
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().NativeWindow());
		int state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS;
	}

	bool WindowInput::IsMouseButtonPressedImpl(int button) const
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().NativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowInput::GetMousePositionImpl() const
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().NativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float WindowInput::GetMousePosXImpl() const
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowInput::GetMousePosYImpl() const
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}