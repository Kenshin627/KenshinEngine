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
			ImGui::DragFloat3("transform", &transform[3][0], 0.1f);
		}

		if (entity.HasComponent<SpiriteRendererComponent>())
		{
			glm::vec4& color = entity.GetComponent<SpiriteRendererComponent>().Color;
			ImGui::ColorEdit4("Color", &color.x);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentSelectedType = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow, "Camera"))
			{
				ImGui::Checkbox("Primary", &cameraComponent.Primary);
				ImGui::Checkbox("FixAspectRatio", &cameraComponent.FixedAspectRatio);

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

				ImGui::TreePop();
			}
		}
	}
}