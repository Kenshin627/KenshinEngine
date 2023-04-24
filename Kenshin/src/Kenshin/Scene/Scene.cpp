#include "kspch.h"
#include "Scene.h"
#include "Kenshin/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "Kenshin/Scene/CameraController.h"

namespace Kenshin
{
	Scene::Scene() 
	{
		Entity entity = CreateEntity("blueQuad");
		entity.AddComponent<SpiriteRendererComponent>(0.0f, 0.0f, 1.0f, 1.0f);

		Entity entity1 = CreateEntity("redQuad");
		entity1.AddComponent<SpiriteRendererComponent>(0.8f, 0.1f, 0.1f, 1.0f);
		entity1.Replace<TransformComponent>(glm::translate(glm::mat4(1.0f), { -2.0f, 0.0f, 0.0f }));

		//SceneCamera
		Entity mainCamera = CreateEntity("CameraA");
		auto& cameraAComponent = mainCamera.AddComponent<CameraComponent>();
		cameraAComponent.Primary = true;
		cameraAComponent.FixedAspectRatio = false;
		NativeScriptComponent& cameraScripts = mainCamera.AddComponent<NativeScriptComponent>();
		cameraScripts.Bind<CameraController>(mainCamera);
		cameraScripts.OnCreate(cameraScripts.Instance);

		Entity secondCamera = CreateEntity("CameraB");
		auto& cameraBComponent = secondCamera.AddComponent<CameraComponent>();
		cameraBComponent.Primary = false;
		cameraBComponent.FixedAspectRatio = true;
	}
	Scene::~Scene() 
	{
	
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity (m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::OnUpdate(TimeStamp ts)
	{
		
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			auto cameraGroups = m_Registry.view<CameraComponent>();
			for (auto& entity : cameraGroups)
			{
				CameraComponent& cameraComponent = cameraGroups.get<CameraComponent>(entity);
				if (!cameraComponent.FixedAspectRatio)
				{
					cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				}
			}
		}		
	}

	void Scene::RenderScene(TimeStamp ts)
	{
		//updateScripts
		auto scripts = m_Registry.view<NativeScriptComponent>();
		for (auto& entity : scripts)
		{
			NativeScriptComponent& nsc = scripts.get<NativeScriptComponent>(entity);
			nsc.OnUpdate(nsc.Instance, ts);
		}

		Camera* mainCamera = nullptr;
		glm::mat4 transform;
		auto cameraGroup = m_Registry.group<TransformComponent, CameraComponent>();
		for (auto& entity : cameraGroup)
		{
			auto[transformComponent, cameraComponent] = cameraGroup.get<TransformComponent, CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				mainCamera = &cameraComponent.Camera;
				transform = transformComponent.Transform;
				break;
			}
		}

		if (mainCamera)
		{			
			Renderer2D::BeginScene(*mainCamera, transform);
			auto group = m_Registry.view<TransformComponent, SpiriteRendererComponent>();
			for (auto& entity : group)
			{
				glm::mat4 transform = group.get<TransformComponent>(entity).Transform;
				glm::vec4 color = group.get<SpiriteRendererComponent>(entity).Color;
				Renderer2D::DrawQuad(transform, color);
			}
			Renderer2D::EndScene();
		}
	}
}