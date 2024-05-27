#pragma once
#include "Kenshin/Renderer/Shader.h"

namespace Kenshin {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSource, const char* fragmentSource);
		OpenGLShader(const std::string& filePath);
		virtual ~OpenGLShader();

		virtual std::string ReadFile(const std::string& filePath) override;
		virtual std::unordered_map<GLenum, std::string> PreProcess(const std::string& source) override;
		virtual void Compile(const std::unordered_map<GLenum, std::string>& shaders) override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void UploadUniformMat4(const char* name, const glm::mat4& transform) const override;
		virtual void UploadUniformMat3(const char* name, const glm::mat3& transform) const override;

		virtual void UploadUniformFloat(const char* name, float value) const override;
		virtual void UploadUniformFloat2(const char* name, const glm::vec2& value) const override;
		virtual void UploadUniformFloat3(const char* name, const glm::vec3& value) const override;
		virtual void UploadUniformFloat4(const char* name, const glm::vec4& value) const override;

		virtual void UploadUniformInt(const char* name, int transform) const override;
	private:
		uint32_t m_RendererID;
		std::string m_Filepath;
	};
}