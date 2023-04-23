#pragma once
#include "Kenshin/Core/Core.h"
#include "entt.hpp"
#include "Kenshin/Core/TimeStamp.h"

namespace Kenshin
{
	class KENSHIN_API Scene
	{		
	public:
		friend class Entity;
		Scene();
		~Scene();
		void OnUpdate(TimeStamp ts);
		Entity CreateEntity(const std::string& name);
		const entt::registry& Registry() const { return m_Registry; }
		void RenderScene();
	private:
		entt::registry m_Registry;
	};
}