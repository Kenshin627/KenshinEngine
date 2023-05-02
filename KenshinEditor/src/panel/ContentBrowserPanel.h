#pragma once
#include "Kenshin/Renderer/Texture.h"
#include <filesystem>
#include "Kenshin/Core/Core.h"

namespace Kenshin
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDictionary;
		std::filesystem::path m_CurrentDictionary;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_BackIcon;
	};
}