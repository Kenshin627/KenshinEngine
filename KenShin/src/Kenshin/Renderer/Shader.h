#pragma once
#include "Kenshin/Core.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Kenshin {
	class KS_API Shader
	{
	public:
		Shader() {};
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		static Ref<Shader> Create(const std::string& name, const char* vertexSource, const char* fragmentSource);
		static Ref<Shader> Create(const std::string& filePath);
		virtual const std::string& GetName() const = 0;
		virtual std::string ReadFile(const std::string& filePath) = 0;
		virtual std::unordered_map<GLenum, std::string> PreProcess(const std::string& source) = 0;
		virtual void Compile(const std::unordered_map<GLenum, std::string>& shaders) = 0;
		virtual void UploadUniformMat4(const char* name, const glm::mat4& transform) const = 0;
		virtual void UploadUniformMat3(const char* name, const glm::mat3& transform) const = 0;

		virtual void UploadUniformFloat(const char* name, float value) const = 0;
		virtual void UploadUniformFloat2(const char* name, const glm::vec2& value) const = 0;
		virtual void UploadUniformFloat3(const char* name, const glm::vec3& value) const = 0;
		virtual void UploadUniformFloat4(const char* name, const glm::vec4& value) const = 0;

		virtual void UploadUniformInt(const char* name, int transform) const = 0;

	};

	class KS_API ShaderLibrary
	{
	public:
		ShaderLibrary() {}
		~ShaderLibrary() {}
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Get(const std::string& name) const;
		bool Exist(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}