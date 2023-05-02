#include "kspch.h"
#include "ContentBrowerPanel.h"
#include "imgui.h"

namespace Kenshin
{
	static char* asset_Path = "resource";
	ContentBrowerPanel::ContentBrowerPanel() :m_BaseDictionary(asset_Path), m_CurrentDictionary(asset_Path) 
	{
		m_DirectoryIcon = Texture2D::Create("resource/contentBrower/icons/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resource/contentBrower/icons/FileIcon.png");
	}

	void ContentBrowerPanel::OnImGuiRender() 
	{
		ImGui::Begin("Content Brower");
		ImGui::End();
	}
}