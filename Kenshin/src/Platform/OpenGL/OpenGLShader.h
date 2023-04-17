#pragma once
#include "Kenshin/Renderer/Shader.h"
#include <glm.hpp>
#include <unordered_map>

namespace Kenshin
{
	class  OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void unBind() override;
		virtual void SetInt(const std::string& key, int val) const override;
		virtual void SetFloat(const std::string& key, float val) const override;
		virtual void SetVec2(const std::string& key, const glm::vec2& v2) const override;
		virtual void SetVec3(const std::string& key, const glm::vec3& v3) const override;
		virtual void SetVec4(const std::string& key, const glm::vec4& v4) const override;
		virtual void SetMat2(const std::string& key, const glm::mat2& m2) const override;
		virtual void SetMat3(const std::string& key, const glm::mat3& m3) const override;
		virtual void SetMat4(const std::string& key, const glm::mat4& m4) const override;
		virtual std::string GetName() const override;
	private:
		static unsigned ShaderTypeFromString(const std::string& type);
		std::string ReadShader(const std::string& path);
		std::unordered_map<unsigned, std::string> PreProcess(const std::string& source);
		void CompileShader(const std::unordered_map<unsigned, std::string>& sources);
		int GetUniformLocation(const std::string& key) const;
	private:
		unsigned m_RendererID;
		std::unordered_map<std::string, int> uniformLocationCaches;
		std::string m_Path;
	};
}