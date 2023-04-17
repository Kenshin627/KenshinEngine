#include "kspch.h"
#include "OpenGLRendererAPI.h"
#include <glad/gl.h>

namespace Kenshin
{
	OpenGLRendererAPI::OpenGLRendererAPI() {}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (const void*)nullptr);
	}

	void OpenGLRendererAPI::ResizeViewport(size_t x, size_t y, size_t width, size_t height)
	{
		glViewport(x, y, width, height);
	}
}