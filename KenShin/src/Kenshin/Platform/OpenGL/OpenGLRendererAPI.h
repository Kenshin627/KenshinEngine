#pragma once
#include "Kenshin/Renderer/RendererAPI.h"

namespace Kenshin {
	class OpenGLRendererAPI: public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void Clear() const override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) const override;
	};
}