#pragma once
namespace Kenshin {
	class Shader
	{
	public:
		Shader(const char* vertexSource, const char* fragmentSource);
		~Shader();
		uint32_t CompileShader(uint32_t shaderType, const char* source) const;
		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RendererID;
	};
}