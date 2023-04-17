#pragma once
#include "Kenshin/Core.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "glm.hpp"
namespace Kenshin
{
	class KENSHIN_API RendererCommand
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
		void static DrawIndexed(const Ref<VertexArray>& vertexArray) 
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		};

		void static ResizeViewport(size_t x, size_t y, size_t width, size_t height)
		{
			s_RendererAPI->ResizeViewport(x, y, width, height);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}