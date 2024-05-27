#include "kspch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Kenshin/Platform/OpenGL/OpenGLTexture.h"

namespace Kenshin {
	Ref<Texture2D> Texture2D::Create(const char* path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KS_ASSET(false, "unsupport RendererAPI!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}
		KS_ASSET(false, "unsupport RendererAPI!"); 
		return nullptr;
	}
}