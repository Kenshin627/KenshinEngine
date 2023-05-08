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
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t indicesCount = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (const void*)nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, float lineWidth)
	{
		vertexArray->Bind();
		glLineWidth(lineWidth);
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::ResizeViewport(size_t x, size_t y, size_t width, size_t height)
	{
		glViewport(x, y, width, height);
	}
}