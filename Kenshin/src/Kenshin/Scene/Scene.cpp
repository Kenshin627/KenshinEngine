#include "kspch.h"
#include "Scene.h"
#include "Kenshin/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

class Entity;
namespace Kenshin
{
	Scene::Scene() {}
	Scene::~Scene() {}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this, name };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>();
		return entity;
	}

	void Scene::OnUpdate(TimeStamp ts)
	{
		
	}

	void Scene::RenderScene()
	{
		auto group = m_Registry.group<TransformComponent, SpiriteRendererComponent>();
		for (auto& entity : group)
		{
			glm::mat4 transform = group.get<TransformComponent>(entity).Transform;
			glm::vec4 color = group.get<SpiriteRendererComponent>(entity).Color;
			Renderer2D::DrawQuad(transform, color);
		}
	}
}