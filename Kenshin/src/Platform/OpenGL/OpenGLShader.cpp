#include "kspch.h"
#include "OpenGLShader.h"
#include<glad/gl.h>

namespace Kenshin
{
	OpenGLShader::OpenGLShader(const std::string& vertex, const std::string& fragment, const std::string& geometry) :m_RendererID(0)
	{
		int success;
		std::array<char, 512> msg;
		m_RendererID = glCreateProgram();
		unsigned vertexID = CompileShader(vertex, GL_VERTEX_SHADER);
		unsigned fragmentID = CompileShader(fragment, GL_FRAGMENT_SHADER);
		unsigned geometryID = 0;
		if (!geometry.empty())
		{
			unsigned geometryID = CompileShader(geometry, GL_GEOMETRY_SHADER);
			glAttachShader(m_RendererID, geometryID);
		}

		glAttachShader(m_RendererID, vertexID);
		glAttachShader(m_RendererID, fragmentID);
		glLinkProgram(m_RendererID);
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_RendererID, 512, nullptr, msg.data());
			KS_CORE_ERROR("program link failed: {0}", msg.data());
		}
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		if (geometryID != 0)
		{
			glDeleteShader(geometryID);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	unsigned OpenGLShader::CompileShader(const std::string& path, unsigned shaderType)
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
			const char* vShaderCode = shaderCode.c_str();
			unsigned shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &vShaderCode, nullptr);
			glCompileShader(shaderID);
			int success;
			std::array<char, 512> msg;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderID, 512, nullptr, msg.data());
				KS_CORE_ERROR("{0} shader compile failed: {1}", (shaderType == GL_VERTEX_SHADER ? "VERTEX" : shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY"), msg.data());
			}
			return shaderID;
		}
		catch (const std::exception& e)
		{
			KS_CORE_ERROR("ShaderFile not read successfully, {0}", e.what());
			return -1;
		}
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
}