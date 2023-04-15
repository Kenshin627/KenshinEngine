#pragma once
#include "Kenshin/Renderer/RendererAPI.h"

namespace Kenshin
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}