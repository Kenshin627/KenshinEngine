#pragma once
#include "Scene.h"

namespace Kenshin
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		void Serialize(const std::string& filePath);
		bool DeSerialize(const std::string& filePath);
	private:
		Ref<Scene> m_Scene;
	};
}