#include "kspch.h"
#include "OpenGLContext.h"

#include <glad/gl.h>

namespace Kenshin
{
	OpenGLContext::OpenGLContext(GLFWwindow* windwHandle) :m_WindowHandle(windwHandle) {}
	OpenGLContext::~OpenGLContext() = default;
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		KS_CORE_ASSERT(status, "Failed to initailize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}