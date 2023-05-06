#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Scene/Scene.h"
#include "Kenshin/Scene/Entity.h"
#include "Kenshin/Renderer/Texture.h"


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
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction func);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		Ref<Texture2D> m_DefaultTextureSlot = Texture2D::Create("resource/textures/Checkerboard.png");
		Ref<Texture2D> m_SettingTexture = Texture2D::Create("resource/\propertiesPanel/setting.png");
	};
}