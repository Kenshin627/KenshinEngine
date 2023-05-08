#pragma once
#include "Kenshin/Core/Core.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "glm.hpp"

namespace Kenshin
{
	class  RendererCommand
	{
	public:
		void static Init()
		{
			s_RendererAPI->Init();
		}
		void static Clear()
		{
			s_RendererAPI->Clear();
		}
		void static SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		};
		void static DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) 
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		};

		void static DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, float lineWidth)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount, lineWidth);
		}

		void static ResizeViewport(size_t x, size_t y, size_t width, size_t height)
		{
			s_RendererAPI->ResizeViewport(x, y, width, height);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}