#include "kspch.h"
#include "FrameBuffer.h"
#include "Kenshin/Renderer/Renderer.h"
#include "Kenshin/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Kenshin {
	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   KS_CORE_ASSET(false, "RendererAPI::None is not supported!") return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		return nullptr;
	}
}