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
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity CreateEntity(const std::string& name);
		entt::registry& Registry() { return m_Registry; }
		void RenderScene();
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;
	};
}