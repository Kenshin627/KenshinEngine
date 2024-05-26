#pragma once
#include "RendererAPI.h"

namespace Kenshin {

	class RenderCommand
	{
	public:
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
		inline static RendererAPI::API GetAPI() { return s_RenderAPI->GetAPI(); }
	private:
		static RendererAPI* s_RenderAPI;
	};
}