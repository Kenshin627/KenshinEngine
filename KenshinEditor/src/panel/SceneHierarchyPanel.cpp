#include "kspch.h"
#include "SceneHierarchyPanel .h"
#include "Kenshin/Scene/Components.h"
#include <imgui.h>

namespace Kenshin
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) 
	{
		if (scene)
		{
			SetContext(scene);
		}
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (m_Context)
		{
			ImGui::Begin("Scene Hierachy");
			m_Context->Registry().each([&](auto id) {				
				Entity entity { id, m_Context.get() };				
				DrawEntityNode({ entity, m_Context.get() });								
			});
			ImGui::End();
		}

		if (m_SelectionContext)
		{
			ImGui::Begin("properties");
			DrawPropertyPanel(std::forward<Entity>(m_SelectionContext));
			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity&& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected:ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool isOpend = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
		if (isOpend)
		{
			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
	}

	void SceneHierarchyPanel::DrawPropertyPanel(Entity&& entity)
	{		
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			glm::mat4& transform = entity.GetComponent<TransformComponent>().Transform;
			ImGui::DragFloat3("##transform", &transform[3][0], 0.5f);
		}
	}
}