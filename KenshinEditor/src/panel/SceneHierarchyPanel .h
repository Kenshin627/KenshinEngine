#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Scene/Scene.h"
#include "Kenshin/Scene/Entity.h"


namespace Kenshin
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		~SceneHierarchyPanel() = default;
		void SetContext(const Ref<Scene>& scene);
		void DrawEntityNode(Entity&& entity);
		void DrawPropertyPanel(Entity&& entity);
		void OnImGuiRender();
		template<typename T>
		void DisplayAddComponentEntity(const std::string& name);
		Entity GetSelectedEntity();
		void SetSelectiedEntity(const Entity& entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}