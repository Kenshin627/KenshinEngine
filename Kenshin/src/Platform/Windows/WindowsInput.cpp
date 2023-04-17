#include "kspch.h"
#include "WindowsInput.h"
#include "Kenshin/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Kenshin
{
	WindowsInput::WindowsInput() {}
	WindowsInput::~WindowsInput() {}

	bool WindowsInput::IsKeyPressedImpl(KeyCode key)
	{
		int state = glfwGetKey(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMousePressedImpl(MouseCode button)
	{
		int state = glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), &x, &y);
		return std::make_pair(float(x), float(y));
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePos();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePos();
		return y;
	}
}