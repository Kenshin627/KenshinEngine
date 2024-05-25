#include "kspch.h"
#include "Kenshin/Log.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

namespace Kenshin {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :m_WindowHandle(windowHandle) {}
	void OpenGLContext::Init() const
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
		KS_CORE_ASSET(status, "could not intialized Glad!");
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}