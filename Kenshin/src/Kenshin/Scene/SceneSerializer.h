#pragma once
#include "Scene.h"

namespace Kenshin
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene) :m_Scene(scene) {}
		void Serialize(const std::string& filePath);
		void DeSerialize(const std::string& filePath);
	private:
		Ref<Scene> m_Scene;
	};
}