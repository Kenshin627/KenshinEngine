#include "kspch.h"
#include "WindowsWindow.h"

namespace Kenshin
{
	static bool s_GLFWInitialized = false;
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.title;
		m_Data.Width = props.width;
		m_Data.Height = props.height;

		KS_CORE_INFO("creating window {0}, {1}, {2}", props.title, props.width, props.height);
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			KS_CORE_ASSERT(success, "Counld not inialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}
	
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.IsVSync = enabled;
	}

	bool WindowsWindow::IsVSync() const 
	{
		return m_Data.IsVSync;
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}
}