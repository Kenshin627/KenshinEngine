#pragma once

#include "SceneCamera.h"
#include "Kenshin/Renderer/Texture.h"
#include "Kenshin/Core/UUID.h"

#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL 
#include <gtx/quaternion.hpp>

namespace Kenshin
{
	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const IDComponent& rhs) = default;
	};

	struct TagComponent
	{
		std::string Tag{ "Tag" };
		TagComponent() = default;
		TagComponent(const TagComponent& rhs) = default;
		TagComponent(const std::string& tag) :Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& rhs) = default;
		TransformComponent(const glm::vec3& translation) :Translation(translation) {}
		glm::mat4 GetTransform() const 
		{
			return glm::translate(glm::mat4(1.0f), Translation) 
				* glm::toMat4(glm::quat(Rotation)) 
				* glm::scale(glm::mat4(1.0f), Scale);
		}
		operator glm::mat4() { return GetTransform(); }
	};

	struct SpiriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> Texture = nullptr;
		float TilingFactor = 1.0f;
		SpiriteRendererComponent() = default;
		SpiriteRendererComponent(const SpiriteRendererComponent&) = default;
		SpiriteRendererComponent(const glm::vec4& color) :Color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color = { 0.0, 0.9, 1.0, 1.0 };
		float Thinness = 0.02f;
		float Fade = 0.02f;
		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent& rhs) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent& rhs) = default;
	};

	class ScriptableEntity;

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

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		float Damping = 0.01f;
		float GravityScale = 0.0f;
		bool FixedRotation = false;
		bool CCD = false;
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		bool  Visualize = true;
		void* RuntimeFixture = nullptr;
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		bool  Visualize = true;
		void* RuntimeFixture = nullptr;
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	template<typename...Component>
	struct ComponentGroup
	{

	};

	using AllComponents = ComponentGroup<TransformComponent, SpiriteRendererComponent, CircleRendererComponent, CameraComponent, NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;
}