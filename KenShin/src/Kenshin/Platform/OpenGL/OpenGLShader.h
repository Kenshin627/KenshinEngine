#pragma once
#include "Kenshin/Renderer/Shader.h"

namespace Kenshin {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSource, const char* fragmentSource);
		virtual ~OpenGLShader();
		virtual uint32_t CompileShader(uint32_t shaderType, const char* source) const override;
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void UploadUniformMat4(const char* name, const glm::mat4& transform) const override;
	private:
		uint32_t m_RendererID;
	};
}