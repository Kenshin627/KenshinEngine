#include "kspch.h"
#include "RenderCommand.h"
#include "Kenshin/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Kenshin {

	RendererAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI;

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		s_RenderAPI->SetClearColor(color);
	}

	void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}
	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		s_RenderAPI->DrawIndexed(vertexArray);
	}
}