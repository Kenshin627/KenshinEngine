#include "kspch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Kenshin
{
	VertexArray* VertexArray::CreateVertexArray()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None: KS_CORE_ASSERT(false, "not support none rendererAPI!"); return nullptr;
		case RenderAPI::OpenGL: return new OpenGLVertexArray();
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}