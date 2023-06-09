#include "kspch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Kenshin
{
	Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return  CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return  CreateRef<OpenGLVertexBuffer>(size);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not suppoted!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}