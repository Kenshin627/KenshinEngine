#include "kspch.h"
#include "ContentBrowserPanel.h"
#include "imgui.h"

namespace Kenshin
{
	static char* asset_Path = "resource";
	ContentBrowserPanel::ContentBrowserPanel() :m_BaseDictionary(asset_Path), m_CurrentDictionary(asset_Path)
	{
		m_DirectoryIcon = Texture2D::Create("resource/contentBrower/icons/folder01.png");
		m_FileIcon = Texture2D::Create("resource/contentBrower/icons/file.png");
		m_BackIcon = Texture2D::Create("resource/contentBrower/icons/back.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		
		ImGui::Begin("Content Brower", nullptr, 0 | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
		if (m_CurrentDictionary != m_BaseDictionary)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));			
			if (ImGui::ImageButton((ImTextureID)m_BackIcon->GetRendererID(), { 20, 20 }, { 0,1 }, { 1,0 }))
			{
				m_CurrentDictionary = m_CurrentDictionary.parent_path();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
			ImGui::PopStyleColor(2);
		}

		static float padding = 32.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
		{
			columnCount = 1;
		}
		ImGui::BeginTable("content", columnCount);
		
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDictionary))
		{
			ImGui::TableNextColumn();
			bool isDirectory = directoryEntry.is_directory();
			const auto& path = directoryEntry.path();
			std::string fileName = path.filename().string();

			ImGui::PushID(fileName.c_str());
			Ref<Texture2D> icon = isDirectory ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 16));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			ImGui::PopStyleColor(2);
			if (!isDirectory)
			{
				if (ImGui::BeginDragDropSource())
				{
					std::filesystem::path relativePath(path);
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::Text(relativePath.filename().string().c_str());
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && isDirectory)
			{
				m_CurrentDictionary /= path.filename();
			}
			ImGui::Text(fileName.c_str());			
			ImGui::PopStyleVar();
			ImGui::PopID();
		}
		ImGui::EndTable();
		ImGui::End();
	}
}