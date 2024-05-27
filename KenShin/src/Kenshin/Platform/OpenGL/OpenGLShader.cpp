#include "kspch.h"
#include "Kenshin/Log.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Kenshin {
	
	static int ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
		KS_CORE_ERROR("unknown shader type: {0}", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath):m_Filepath(filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const char* vertexSource, const char* fragmentSource):m_Filepath(""), m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shadersources;
		shadersources[GL_VERTEX_SHADER] = vertexSource;
		shadersources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shadersources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		//Extract fileName from path
		size_t lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filePath.find_last_of(".");
		size_t count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
		std::ifstream in(filePath.c_str(), std::ios::in, std::ios::binary);
		std::string result;
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			KS_CORE_ERROR("read file failed! {0}", filePath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaders;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			KS_CORE_ASSET(eol, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			int glShaderType = ShaderTypeFromString(type);
			KS_CORE_ASSET(glShaderType, "unknown shader type!");
			size_t nextLinePos = source.find_first_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaders[glShaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		for (auto& i : shaders)
		{
			KS_CORE_WARN(i.second);
		}
		return shaders;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
	{		
		uint32_t program = glCreateProgram();
		
		std::array<GLenum, 2> shaderIds;

		uint32_t index = 0;
		for (auto& i : shaders)
		{
			int shaderType = i.first;
			const char* shaderSource = i.second.c_str();
			uint32_t shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &shaderSource, nullptr);
			glCompileShader(shaderID);
			int status = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (!status)
			{
				int length = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)alloca(length * sizeof(char));
				glGetShaderInfoLog(shaderID, length * sizeof(char), 0, message);
				KS_CORE_ERROR("{0} shader compile failure! {1}", shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", message);
				glDeleteShader(shaderID);
				
			}
			shaderIds[index++] = shaderID;
		}

		for (auto& id : shaderIds)
		{
			glAttachShader(program, id);
		}

		for (auto& id : shaderIds)
		{
			glDetachShader(m_RendererID, id);
		}

		glLinkProgram(program);
		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat4(const char* name, const glm::mat4& transform) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
	}

	void OpenGLShader::UploadUniformMat3(const char* name, const glm::mat3& transform) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(transform));
	}

	void OpenGLShader::UploadUniformFloat(const char* name, float value) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const char* name, const glm::vec2& value) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const char* name, const glm::vec3& value) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const char* name, const glm::vec4& value) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformInt(const char* name, int value) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniform1i(location, value);
	}
}