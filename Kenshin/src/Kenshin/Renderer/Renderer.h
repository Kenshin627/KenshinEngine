#pragma once
#include "VertexArray.h"
#include "RendererCommand.h"

namespace Kenshin
{
	class KENSHIN_API Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray);
	};
}