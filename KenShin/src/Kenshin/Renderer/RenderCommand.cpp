#include "kspch.h"
#include "RenderCommand.h"
#include "Kenshin/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Kenshin {

	RendererAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI;

	void RenderCommand::Init()
	{
		s_RenderAPI->Init();
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RenderAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		s_RenderAPI->SetClearColor(color);
	}

	void RenderCommand::Clear()
	{
		s_RenderAPI->Clear();
	}
	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		s_RenderAPI->DrawIndexed(vertexArray);
	}
}