#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Core/UUID.h"
#include "Kenshin/Core/TimeStamp.h"
#include "entt.hpp"
#include "SceneCamera.h"
#include "EditorCamera.h"


class b2World;
namespace Kenshin
{
	class Entity;
	class  Scene
	{		
	public:
		friend class Entity;
		Scene();
		~Scene();
		void OnUpdateEditor(TimeStamp ts, const EditorCamera& camera);
		void OnUpdateRuntime(TimeStamp ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
		void DestroyEntity(Entity entity);
		entt::registry& Registry() { return m_Registry; }
		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnPhysics2DStart();
		void OnPhysics2DStop();
		template<typename T>
		void OnEntityAddComponent(Entity* entity, T& component);
		std::pair<glm::mat4, glm::mat4>  GetMainCamera();
		static Ref<Scene> Copy(const Ref<Scene>& other);
		Entity DuplicateEntity(Entity entity);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;
		b2World* m_PhysicsWorld = nullptr;
		friend class EditLayer;
		friend class SceneSerializer;
	};
}