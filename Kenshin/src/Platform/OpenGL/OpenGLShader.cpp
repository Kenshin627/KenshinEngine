#include "kspch.h"
#include "OpenGLShader.h"
#include<glad/gl.h>

namespace Kenshin
{
	OpenGLShader::OpenGLShader(const std::string& path) :m_RendererID(0), m_Path(path)
	{
		std::string source = ReadShader(path);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		CompileShader(shaderSources);
	}

	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		if (type == "geometry") return GL_GEOMETRY_SHADER;
		KS_CORE_ASSERT(false, "shader type not supported!");
	}

	std::string OpenGLShader::ReadShader(const std::string& path)
	{
		std::string shaderCode;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = shaderStream.str();
			return shaderCode;
		}
		catch (const std::exception& e)
		{
			KS_CORE_ERROR("ShaderFile not read successfully, {0}", e.what());
			return nullptr;
		}
	}

	std::unordered_map<unsigned, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<unsigned, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			KS_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			KS_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel" || type == "geometry", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::CompileShader(const std::unordered_map<unsigned, std::string>& shaderSources)
	{
		m_RendererID = glCreateProgram();
		for (auto& [type, source] : shaderSources)
		{
			const char* vShaderCode = source.c_str();
			unsigned shaderID = glCreateShader(type);
			glShaderSource(shaderID, 1, &vShaderCode, nullptr);
			glCompileShader(shaderID);
			int success;
			std::array<char, 512> msg;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderID, 512, nullptr, msg.data());
				KS_CORE_ERROR("{0} shader compile failed: {1}", (type == GL_VERTEX_SHADER ? "VERTEX" : type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY"), msg.data());
			}
			glAttachShader(m_RendererID, shaderID);
			glLinkProgram(m_RendererID);
			glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(m_RendererID, 512, nullptr, msg.data());
				KS_CORE_ERROR("program link failed: {0}", msg.data());
			}
			glDeleteShader(shaderID);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::unBind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& key, int val) const
	{
		glUniform1i(GetUniformLocation(key), val);
	}

	void OpenGLShader::SetFloat(const std::string& key, float val) const
	{
		glUniform1f(GetUniformLocation(key), val);
	}

	void OpenGLShader::SetVec2(const std::string& key, const glm::vec2& v2) const
	{
		glUniform2fv(GetUniformLocation(key), 1, glm::value_ptr(v2));
	}

	void OpenGLShader::SetVec3(const std::string& key, const glm::vec3& v3) const
	{
		glUniform3fv(GetUniformLocation(key), 1, glm::value_ptr(v3));
	}

	void OpenGLShader::SetVec4(const std::string& key, const glm::vec4& v4) const
	{
		glUniform4fv(GetUniformLocation(key), 1, glm::value_ptr(v4));
	}

	void OpenGLShader::SetMat2(const std::string& key, const glm::mat2& m2) const
	{
		glUniformMatrix2fv(GetUniformLocation(key), 1, false, glm::value_ptr(m2));
	}

	void OpenGLShader::SetMat3(const std::string& key, const glm::mat3& m3) const
	{
		glUniformMatrix3fv(GetUniformLocation(key), 1, false, glm::value_ptr(m3));
	}

	void OpenGLShader::SetMat4(const std::string& key, const glm::mat4& m4) const
	{
		glUniformMatrix4fv(GetUniformLocation(key), 1, false, glm::value_ptr(m4));
	}

	int OpenGLShader::GetUniformLocation(const std::string& key) const
	{
		auto iter = uniformLocationCaches.find(key);
		if (iter != uniformLocationCaches.cend())
		{
			return iter->second;
		}
		int location = glGetUniformLocation(m_RendererID, key.c_str());
		KS_CORE_ASSERT(location != -1, "uniform {0} not found", key);
		return location;
	}

	std::string OpenGLShader::GetName() const
	{
		size_t lastSlash = m_Path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = m_Path.rfind(".");
		size_t count = lastDot == std::string::npos ? m_Path.size() - lastSlash : lastDot - lastSlash;
		
		return m_Path.substr(lastSlash , count);
	}
}