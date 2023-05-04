#include "kspch.h"
#include "SceneHierarchyPanel .h"
#include "Kenshin/Scene/Components.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Kenshin/Scene/QuadController.h"
#include <filesystem>

namespace Kenshin
{
	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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
		ImGui::Begin("Scene Hierachy", nullptr, ImGuiWindowFlags_NoCollapse);
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
			if (m_SelectionContext == entity)
			{
				m_SelectionContext = {};
				//m_Context->SetSelectEntity()
			}
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
			DisplayAddComponentEntity<CameraComponent>("Camera Component");
			DisplayAddComponentEntity<Rigidbody2DComponent>("RigidBody Component");
			DisplayAddComponentEntity<BoxCollider2DComponent>("BoxCollider Component");
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotationDegree = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotationDegree);
			component.Rotation = glm::radians(rotationDegree);
			DrawVec3Control("Scale", component.Scale, 1.0f);
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

		DrawComponent<SpiriteRendererComponent>("Spirite", entity, [&](SpiriteRendererComponent& component) {
			glm::vec4& color = component.Color;
			ImGui::ColorEdit4("Color", &color.x);

			ImGui::Text("Texture");
			
			auto textureId = component.Texture ? component.Texture->GetRendererID() : m_DefaultTextureSlot->GetRendererID();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 20);
			ImGui::Image((ImTextureID)textureId, { 64, 64 }, { 0,1 }, { 1,0 });
			ImGui::PopStyleVar();
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* itemPath = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(itemPath);
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					component.Texture = texture;
					ImGui::EndDragDropTarget();
				}				
			}
			ImGui::DragFloat("TilingFactor", &component.TilingFactor, 1.0, 0.1f, 100.0f);
		});

		DrawComponent<Rigidbody2DComponent>("RigidBody", entity, [](Rigidbody2DComponent& component) {	
			const char* rigidBodyType[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentRigidBodyType = rigidBodyType[(int)component.Type];			
			if (ImGui::BeginCombo("RigidBodyType", currentRigidBodyType))
			{
				for (size_t i = 0; i < 3; i++)
				{
					bool isSelected = rigidBodyType[i] == currentRigidBodyType;
					if (ImGui::Selectable(rigidBodyType[i], isSelected, ImGuiSelectableFlags_None))
					{
						currentRigidBodyType = rigidBodyType[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::DragFloat("Damping", &component.Damping, 0.01f, 0.0f, 0.1f);
			ImGui::DragFloat("GravityScale", &component.GravityScale, 0.1, -10.0f, 10.0f);
			ImGui::Checkbox("CCD", &component.CCD);
			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](BoxCollider2DComponent& component) {
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
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

	Entity SceneHierarchyPanel::GetSelectedEntity()
	{
		return m_SelectionContext;
	}

	void SceneHierarchyPanel::SetSelectiedEntity(const Entity& entity)
	{
		m_SelectionContext = entity;
	}
}