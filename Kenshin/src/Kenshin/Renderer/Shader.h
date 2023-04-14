#pragma once
#include <string>
#include <glm.hpp>

namespace Kenshin
{
	class Shader
	{
	public:
		Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry = nullptr);
		~Shader();
		void Bind();
		void unBind();
		void SetInt(const std::string& key, int val) const;
		void SetFloat(const std::string& key, float val) const;
		void SetVec2(const std::string& key, const glm::vec2& v2) const;
		void SetVec3(const std::string& key, const glm::vec3& v3) const;
		void SetVec4(const std::string& key, const glm::vec4& v4) const;
		void SetMat2(const std::string& key, const glm::mat2& m2) const;
		void SetMat3(const std::string& key, const glm::mat3& m3) const;
		void SetMat4(const std::string& key, const glm::mat4& m4) const;
	private:
		unsigned CompileShader(const std::string& path, unsigned shaderType);
		int GetUniformLocation(const std::string& key) const;
	private:
		unsigned m_RendererID;
		std::unordered_map<std::string, int> uniformLocationCaches;
	};
}