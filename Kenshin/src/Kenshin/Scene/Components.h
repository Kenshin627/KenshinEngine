#pragma once
#include <string>
#include <glm.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include <functional>

namespace Kenshin
{
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent& rhs) = default;
		TransformComponent(const glm::mat4& transform) :Transform(transform) {}
		operator glm::mat4() { return Transform; }
	};

	struct TagComponent
	{
		std::string Tag{ "Tag" };
		TagComponent() = default;
		TagComponent(const TagComponent& rhs) = default;
		TagComponent(const std::string& tag) :Tag(tag) {}
	};

	struct SpiriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		SpiriteRendererComponent() = default;
		SpiriteRendererComponent(const SpiriteRendererComponent& rhs) = default;
		SpiriteRendererComponent(const glm::vec4& color) :Color(color) {}
		SpiriteRendererComponent(float r, float g, float b, float a) : Color({ r, g, b, a }) {}
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
		ScriptableEntity* Instance = nullptr;
		std::function<void(ScriptableEntity*)> OnCreate = nullptr;
		std::function<void(ScriptableEntity*)> OnDestroy = nullptr;
		std::function<void(ScriptableEntity*, TimeStamp)> OnUpdate = nullptr;

		template<typename T>
		void Bind(const Entity& entity)
		{
			Instance = new T();
			Instance->SetEntity(entity);
			OnCreate = [](ScriptableEntity* script) { return script->OnCreate(); };
			OnDestroy = [](ScriptableEntity* script) { return script->OnDestroy(); };
			OnUpdate = [](ScriptableEntity* script, TimeStamp ts) { return script->OnUpdate(ts); };
		}
	};
}