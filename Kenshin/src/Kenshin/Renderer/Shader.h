#pragma once
#include <string>
#include <unordered_map>
#include <glm.hpp>

namespace Kenshin
{
	class KENSHIN_API Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind() = 0;
		virtual void unBind() = 0;
		virtual void SetInt(const std::string& key, int val) const = 0;
		virtual void SetFloat(const std::string& key, float val) const = 0;
		virtual void SetVec2(const std::string& key, const glm::vec2& v2) const = 0;
		virtual void SetVec3(const std::string& key, const glm::vec3& v3) const = 0;
		virtual void SetVec4(const std::string& key, const glm::vec4& v4) const = 0;
		virtual void SetMat2(const std::string& key, const glm::mat2& m2) const = 0;
		virtual void SetMat3(const std::string& key, const glm::mat3& m3) const = 0;
		virtual void SetMat4(const std::string& key, const glm::mat4& m4) const = 0;
		virtual std::string GetName() const = 0;
		static Ref<Shader> Create(const std::string& path);
	};

	class KENSHIN_API ShaderLibrary
	{
	public:
		ShaderLibrary() = default;
		~ShaderLibrary() = default;
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& path);
		Ref<Shader> Load(const std::string& name, const std::string& path);
		Ref<Shader> Get(const std::string& name);
	private:
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}