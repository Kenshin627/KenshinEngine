#pragma once
#include "Kenshin/Renderer/Texture.h"
#include <filesystem>
#include "Kenshin/Core/Core.h"

namespace Kenshin
{
	class ContentBrowerPanel
	{
	public:
		ContentBrowerPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDictionary;
		std::filesystem::path m_CurrentDictionary;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}