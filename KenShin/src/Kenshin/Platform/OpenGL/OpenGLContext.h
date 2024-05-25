#pragma once
#include "Kenshin/Renderer/GraphicContext.h"

struct GLFWwindow;
namespace Kenshin {
	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() const override;
		virtual void SwapBuffers() const override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}