#include "kspch.h"
#include "Buffer.h"
#include "Kenshin/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Kenshin
{
	VertexBuffer* VertexBuffer::CreateBuffer(const float* vertices, unsigned size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::None: KS_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
			case RenderAPI::OpenGL: return  new OpenGLVertexBuffer(vertices, size);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::CreateBuffer(const unsigned* indices, unsigned count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None: KS_CORE_ASSERT(false, "RendererAPI::None not suppoted!"); return nullptr;
		case RenderAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}