#include "kspch.h"
#include "Kenshin/Core.h"
#include "Kenshin/Log.h"
#include "Buffer.h"
#include "Kenshin/Platform/OpenGL/OpenGLBuffer.h"
#include "Kenshin/Renderer/Renderer.h"

namespace Kenshin {
	VertexBuffer* VertexBuffer::Create(float* data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:   KS_CORE_ASSET(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(data, size);
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* data, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:   KS_CORE_ASSET(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(data, count);
		}
		return nullptr;
	}
}