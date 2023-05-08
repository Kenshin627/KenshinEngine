#pragma once
#include <glm.hpp>
#include "Kenshin/Core/Core.h"
#include "VertexArray.h"

namespace Kenshin
{
	class RendererAPI
	{		
	public:
		enum API
		{
			None = 0, OpenGL
		};
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, float lineWidth) = 0;
		virtual void ResizeViewport(size_t x, size_t y, size_t width, size_t height) = 0;
		inline static API GetAPI() { return s_Api; };
	private:
		static API s_Api;
	};
}