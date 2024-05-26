#include "kspch.h"
#include "VertexArray.h"
#include "Kenshin/Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Kenshin {
	VertexArray::VertexArray() {}
	VertexArray::~VertexArray() {}
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    KS_ASSET(false, "unkown support RendererAPI!"); return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}
		KS_ASSET(false, "unkown support RendererAPI!");
		return nullptr;
	}
}