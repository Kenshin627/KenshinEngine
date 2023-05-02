#include "kspch.h"
#include "ContentBrowserPanel.h"
#include "imgui.h"

namespace Kenshin
{
	static char* asset_Path = "resource";
	ContentBrowserPanel::ContentBrowserPanel() :m_BaseDictionary(asset_Path), m_CurrentDictionary(asset_Path)
	{
		m_DirectoryIcon = Texture2D::Create("resource/contentBrower/icons/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resource/contentBrower/icons/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Content Brower");
		if (m_CurrentDictionary != m_BaseDictionary)
		{
			if (ImGui::Button("<-")) 
			{
				m_CurrentDictionary = m_CurrentDictionary.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 80.0f;
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
			const auto& path = directoryEntry.path();
			std::string fileName = path.filename().string();

			ImGui::PushID(fileName.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory()) 
				{
					m_CurrentDictionary /= path.filename();
				}
			}
			
			ImGui::Text(fileName.c_str());			
			ImGui::PopID();
		}
		ImGui::EndTable();
		ImGui::End();
	}
}