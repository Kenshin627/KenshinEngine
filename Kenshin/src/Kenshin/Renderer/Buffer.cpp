#include "kspch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Kenshin
{
	VertexBuffer* VertexBuffer::CreateBuffer(const float* vertices, unsigned size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return  new OpenGLVertexBuffer(vertices, size);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::CreateBuffer(const unsigned* indices, unsigned count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not suppoted!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}