#include "kspch.h"
#include "Kenshin/Log.h"
#include "Kenshin/events/ApplicationEvent.h"
#include "Kenshin/events/KeyEvent.h"
#include "Kenshin/events/MouseEvent.h"
#include "Windowwindow.h"
#include <glad/glad.h>

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

	void* Windowwindow::NativeWindow()
	{
		return m_Window;
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
			//glad initialize
			int status = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
			KS_CORE_ASSET(status, "could not intialized Glad!");
			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(true);

			//callbacks
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				data.Width = width;
				data.Height = height;
				WindowResizeEvent e(width, height);
				data.Callback(e);
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				WindowCloseEvent e;
				data.Callback(e);
			});

			glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				WindowFocusEvent e;
				data.Callback(e);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent keypress(key, 0);
					data.Callback(keypress);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent keyrelease(key);
					data.Callback(keyrelease);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent keyrepeat(key, 1);
					data.Callback(keyrepeat);
					break;
				}
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				KeyTypedEvent e(keycode);
				data.Callback(e);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				switch (action)
				{
					case GLFW_PRESS:
					{
						MousePressedEvent mousepress(button);
						data.Callback(mousepress);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseReleaseEvent mouserelease(button);
						data.Callback(mouserelease);
						break;
					}
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				MouseScrollEvent e(xoffset, yoffset);
				data.Callback(e);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				MouseMoveEvent e(xpos, ypos);
				data.Callback(e);
			});
	}

	void Windowwindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}