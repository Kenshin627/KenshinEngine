#include "kspch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Kenshin
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return  CreateRef<OpenGLFrameBuffer>(spec);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}
}