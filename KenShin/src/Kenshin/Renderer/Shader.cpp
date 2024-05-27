#include "kspch.h"
#include "Kenshin/Log.h"
#include "Shader.h"
#include "Renderer.h"
#include "Kenshin/Platform/OpenGL/OpenGLShader.h"

namespace Kenshin {
	Ref<Shader> Shader::Create(const std::string& name, const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KS_ASSET(false, "unknown supported RendereraPI!"); return __nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
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

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		KS_CORE_ASSET(!Exist(name), "shader already Exist!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		KS_CORE_ASSET(!Exist(name), "shader already Exist!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) const
	{
		KS_CORE_ASSET(Exist(name), "shader not found!");
		return m_Shaders.find(name)->second;
	}

	bool ShaderLibrary::Exist(const std::string& name) const
	{		
		return m_Shaders.find(name) != m_Shaders.end();
	}
}