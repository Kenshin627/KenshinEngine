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
		KS_CORE_TRACE("OpenGL Info:");
		KS_CORE_INFO("VENDOR:   {0}", (const char*)glGetString(GL_VENDOR));
		KS_CORE_INFO("RENDERER: {0}", (const char*)glGetString(GL_RENDERER));
		KS_CORE_INFO("VERSION:  {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}