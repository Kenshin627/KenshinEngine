#include "kspch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Kenshin
{
	VertexArray* VertexArray::CreateVertexArray()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "not support none rendererAPI!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}