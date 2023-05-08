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
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, float lineWidth) override;
		virtual void ResizeViewport(size_t x, size_t y, size_t width, size_t height) override;
	};
}