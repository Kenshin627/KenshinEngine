#pragma once
#include "Kenshin/Core.h"
#include <glm/glm.hpp>

namespace Kenshin {
	class KS_API Shader
	{
	public:
		Shader() {};
		virtual ~Shader() {};
		virtual uint32_t CompileShader(uint32_t shaderType, const char* source) const = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		static std::shared_ptr<Shader> Create(const char* vertexSource, const char* fragmentSource);
		virtual void UploadUniformMat4(const char* name, const glm::mat4& transform) const = 0;
	};
}