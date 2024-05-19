#include "kspch.h"
#include "Windowwindow.h"
#include "Kenshin//Log.h"

namespace Kenshin {
	static bool s_GLFWInitialied = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new Windowwindow(props);
	}

	Windowwindow::Windowwindow(const WindowProps& props)
	{
		Init(props);
	}

	Windowwindow::~Windowwindow()
	{
		Shutdown();
	}

	void Windowwindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Windowwindow::SetVSync(bool enable)
	{
		if (enable)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.IsVSync = enable;
	}

	void Windowwindow::Init(const WindowProps& props)
	{
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Title = props.Title;
		KS_CORE_INFO("Create window {0}, {1}, {2}", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialied)
		{
			int success = glfwInit();
			KS_CORE_ASSET(success, "could not intialized GLFW!");
			s_GLFWInitialied = true;
		}
			//asset
			glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);
			if(!m_Window)
			{
				glfwTerminate();
			}
			glfwMakeContextCurrent(m_Window);
			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(true);
	}

	void Windowwindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}