#pragma once
#include "Kenshin/Core.h"
#include "RendererAPI.h"

namespace Kenshin {

	class KS_API RenderCommand
	{
	public:
		static void Init();
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const Ref<VertexArray>& vertexArray);
		inline static RendererAPI::API GetAPI() { return s_RenderAPI->GetAPI(); }
	private:
		static RendererAPI* s_RenderAPI;
	};
}