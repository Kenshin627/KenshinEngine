#pragma once
#include "Kenshin/Core/Core.h"
#include "entt.hpp"
#include "Scene.h"

namespace Kenshin
{
	class Entity
	{
	public:
		Entity(entt::entity id = entt::null, Scene* scene = nullptr, const std::string& name = "Entity");

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T, typename...Args>
		T& AddComponent(Args&&...args)
		{
			KS_CORE_ASSERT(!HasComponent<T>(), "entity has owned the component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename...Args>
		T& Replace(Args&&...args)
		{
			KS_CORE_ASSERT(HasComponent<T>(), "entity not exists in the entity!");
			return m_Scene->m_Registry.replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename...Args>
		T& AddOrReplace(Args&&...args)
		{
			return m_Scene->m_Registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			KS_CORE_ASSERT(HasComponent<T>(), "entity not exists in the entity!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void DeleteComponent()
		{
			KS_CORE_ASSERT(HasComponent<T>(), "entity has not the component!");
			m_Scene->m_Registry.erase<T>(m_EntityHandle);
		}

		operator bool() const
		{
			return m_EntityHandle != entt::null;
		}

		bool operator ==(const Entity& rhs) const
		{
			return m_EntityHandle == rhs.m_EntityHandle && m_Scene == rhs.m_Scene;
		}

		bool operator!=(const Entity& rhs) const
		{
			return !(*this == rhs);
		}

		operator entt::entity() { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene;
	};
}