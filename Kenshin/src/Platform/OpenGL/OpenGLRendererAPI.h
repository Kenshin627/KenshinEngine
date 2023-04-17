#pragma once
#include "Kenshin/Renderer/RendererAPI.h"

namespace Kenshin
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		virtual void Init() override;
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void ResizeViewport(size_t x, size_t y, size_t width, size_t height) override;
	};
}