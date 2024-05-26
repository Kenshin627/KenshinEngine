#pragma once
#include <glm/glm.hpp>

namespace Kenshin {
	class Shader
	{
	public:
		Shader(const char* vertexSource, const char* fragmentSource);
		~Shader();
		uint32_t CompileShader(uint32_t shaderType, const char* source) const;
		void Bind() const;
		void UnBind() const;
		void UploadUniformMat4(const char* name, const glm::mat4& transform) const;
	private:
		uint32_t m_RendererID;
	};
}