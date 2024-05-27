#include "kspch.h"
#include "Kenshin/Log.h"
#include "Shader.h"
#include "Renderer.h"
#include "Kenshin/Platform/OpenGL/OpenGLShader.h"

namespace Kenshin {
	Ref<Shader> Shader::Create(const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KS_ASSET(false, "unknown supported RendereraPI!"); return __nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}
		KS_ASSET(false, "unknown supported RendereraPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: KS_ASSET(false, "unknown supported RendereraPI!"); return __nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}
		KS_ASSET(false, "unknown supported RendereraPI!");
		return nullptr;
	}
}