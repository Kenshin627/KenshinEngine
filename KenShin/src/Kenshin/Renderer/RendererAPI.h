#pragma once
#include<glm/glm.hpp>
#include<memory>
#include"Kenshin/Renderer/VertexArray.h"

namespace Kenshin {
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void Clear() const = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) const = 0;
		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}