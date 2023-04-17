#include "kspch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Kenshin
{
	Shader* Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: KS_CORE_ASSERT(false, "RendererAPI::None not suppoted!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(path);
		}
		KS_CORE_ASSERT(false, "unknown RenderAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		std::string name = shader->GetName();
		KS_CORE_ASSERT(!Exists(name), "shader with name{0} has Already exists!", name);
		m_Shaders.insert({ name, shader });
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		KS_CORE_ASSERT(!Exists(name), "shader with name{0} has Already exists!", name);
		m_Shaders.insert({ name, shader });
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& path)
	{
		auto shader = Ref<Shader>(Shader::Create(path));
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
	{
		auto shader = Ref<Shader>(Shader::Create(path));
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		KS_CORE_ASSERT(Exists(name), "shader with name{0} not exists!", name);
		return m_Shaders[name];		
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.cend();
	}
}