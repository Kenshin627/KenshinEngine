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
	private:
		static RendererAPI* s_RendererAPI;
	};
}