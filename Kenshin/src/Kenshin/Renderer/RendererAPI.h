#pragma once
#include <glm.hpp>
#include "Kenshin/Core.h"
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
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		inline static API GetAPI() { return s_Api; };
	private:
		static API s_Api;
	};
}