#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Kenshin
{
	struct TagComponent
	{
		std::string Tag{ "Tag" };
		TagComponent() = default;
		TagComponent(const TagComponent& rhs) = default;
		TagComponent(const std::string& tag) :Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent& rhs) = default;
		TransformComponent(const glm::vec3& translation) :Translation(translation) {}
		glm::mat4& GetTransform() const 
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0f }) *
								 glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f }) *
								 glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f });
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Translation);
			return translation * scale * rotation;
		}
		operator glm::mat4() { return GetTransform(); }
	};

	struct SpiriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		SpiriteRendererComponent() = default;
		SpiriteRendererComponent(const SpiriteRendererComponent&) = default;
		SpiriteRendererComponent(const glm::vec4& color) :Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent& rhs) = default;
	};

	struct NativeScriptComponent
	{
		bool IsBind = false;
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity* (*IsntantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);
		template<typename T>
		void Bind()
		{
			IsntantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			IsBind = true;
		}
	};
}