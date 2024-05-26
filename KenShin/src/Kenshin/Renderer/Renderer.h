#pragma once
#include "RenderCommand.h"

namespace Kenshin {
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	public:
		static RendererAPI::API GetAPI() { return RenderCommand::GetAPI(); }
	};
}