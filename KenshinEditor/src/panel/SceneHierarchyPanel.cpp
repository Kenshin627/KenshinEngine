#include "kspch.h"
#include "SceneHierarchyPanel .h"
#include "Kenshin/Scene/Components.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Kenshin/Scene/QuadController.h"

namespace Kenshin
{
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			//float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float lineHeight = 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				func(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.DeleteComponent<T>();
			}
		}
	}

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
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");
		if (m_Context)
		{
			m_Context->Registry().each([&](auto id) {				
				Entity entity { id, m_Context.get() };				
				DrawEntityNode({ entity, m_Context.get() });								
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_SelectionContext = {};
			}

			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create empty Entity"))
				{
					//BUG: .AddComponent<SpiriteRendererComponent>(glm::vec4(0.2, 0.8, 0.6, 1.0));
					Entity entity = m_Context->CreateEntity();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("properties");
		if (m_SelectionContext)
		{			
			DrawPropertyPanel(std::forward<Entity>(m_SelectionContext));			
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity&& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected:ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool isOpend = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool isDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				isDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (isOpend)
		{
			ImGui::TreePop();
		}

		if (isDeleted)
		{
			m_Context->DestroyEntity(entity);
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

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntity<TagComponent>("Tag Component");
			DisplayAddComponentEntity<TransformComponent>("Transform Component");
			DisplayAddComponentEntity<SpiriteRendererComponent>("Spirite Component");
			DisplayAddComponentEntity<NativeScriptComponent>("Native Script");
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {		
			ImGui::DragFloat3("transform", &component.Transform[3][0], 0.1f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component) {
			auto& camera = component.Camera;
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentSelectedType = projectionTypeStrings[(int)camera.GetProjectionType()];

			ImGui::Checkbox("Primary", &component.Primary);
			ImGui::Checkbox("FixAspectRatio", &component.FixedAspectRatio);

			if (ImGui::BeginCombo("Camera Type", currentSelectedType))
			{
				for (size_t i = 0; i < 2; i++)
				{
					bool isSelected = projectionTypeStrings[i] == currentSelectedType;
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected, ImGuiSelectableFlags_None))
					{
						currentSelectedType = projectionTypeStrings[i];
						camera.SetProjectionType((ProjectionType)i);
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size))
				{
					camera.SetOrthographicSize(size);
				}

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &nearClip))
				{
					camera.SetOrthographicNearClip(nearClip);
				}

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &farClip))
				{
					camera.SetOrthographicFarClip(farClip);
				}
			}

			else if (camera.GetProjectionType() == ProjectionType::Perspective)
			{
				float verticalFov = glm::degrees(camera.GetPerpectiveVerticalFov());
				if (ImGui::DragFloat("Vertical Fov", &verticalFov))
				{
					camera.SetPerpectiveVerticalFov(glm::radians(verticalFov));
				}

				float nearClip = camera.GetPerspectiveNear();
				if (ImGui::DragFloat("Near", &nearClip))
				{
					camera.SetPerspectiveNear(nearClip);
				}

				float farClip = camera.GetPerspectiveFar();
				if (ImGui::DragFloat("Far", &farClip))
				{
					camera.SetPerspectiveFar(farClip);
				}
			}
		});

		DrawComponent<SpiriteRendererComponent>("Spirite", entity, [](SpiriteRendererComponent& component) {
			glm::vec4& color = component.Color;
			ImGui::ColorEdit4("Color", &color.x);
		});
	}

	template <typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntity(const std::string& name)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(name.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}