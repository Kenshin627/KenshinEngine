#pragma once
#include <GLFW/glfw3.h>
#include "Kenshin/Renderer/GraphicsContext.h"

namespace Kenshin
{
	class OpenGLContext :public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}