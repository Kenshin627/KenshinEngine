#include "kspch.h"
#include "Kenshin/Log.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Kenshin {
	Shader::Shader(const char* vertexSource, const char* fragmentSource)
	{
		uint32_t vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
		uint32_t fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		glLinkProgram(m_RendererID);
		glDetachShader(m_RendererID, vertex);
		glDetachShader(m_RendererID, fragment);
	}

	uint32_t Shader::CompileShader(uint32_t shaderType, const char* source) const
	{
		uint32_t handle = glCreateShader(shaderType);
		glShaderSource(handle, 1, &source, nullptr);
		glCompileShader(handle);
		int status = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			int length = 0;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(handle, length * sizeof(char), 0, message);
			KS_CORE_ERROR("{0} shader compile failure! {1}", shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", message);
			glDeleteShader(handle);
			return 0;
		}
		return handle;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const char* name, const glm::mat4& transform) const
	{
		int location = glGetUniformLocation(m_RendererID, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
	}
}