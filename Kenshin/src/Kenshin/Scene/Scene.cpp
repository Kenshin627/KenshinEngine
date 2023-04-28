#include "kspch.h"
#include "Scene.h"
#include "Kenshin/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "Kenshin/Scene/CameraController.h"
#include "Kenshin/Scene/QuadController.h"

namespace Kenshin
{
	Scene::Scene() 
	{
		Entity entity = CreateEntity("blueQuad");
		entity.AddComponent<SpiriteRendererComponent>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		Entity entity1 = CreateEntity("redQuad");
		entity1.AddComponent<SpiriteRendererComponent>(glm::vec4(0.8f, 0.1f, 0.1f, 1.0f));
		entity1.Replace<TransformComponent>(glm::vec3{ -2.0f, 0.0f, 0.0f });

		//SceneCamera
		Entity mainCamera = CreateEntity("CameraA");
		auto& cameraAComponent = mainCamera.AddComponent<CameraComponent>();
		cameraAComponent.Primary = true;
		cameraAComponent.FixedAspectRatio = false;
		NativeScriptComponent& cameraScripts = mainCamera.AddComponent<NativeScriptComponent>();
		cameraScripts.Bind<CameraController>();

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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
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
		m_Registry.view<NativeScriptComponent>().each([&](entt::entity entity, NativeScriptComponent& nsc) {
			if (!nsc.Instance && nsc.IsBind)
			{
				nsc.Instance = nsc.IsntantiateScript();
				nsc.Instance->m_Entity = { entity, this };
				nsc.Instance->OnCreate();
			}

			if (nsc.IsBind && nsc.Instance)
			{
				nsc.Instance->OnUpdate(ts);
			}
		});

		Camera* mainCamera = nullptr;
		glm::mat4 transform;
		auto cameraGroup = m_Registry.group<TransformComponent, CameraComponent>();
		for (auto entity : cameraGroup)
		{
			auto[transformComponent, cameraComponent] = cameraGroup.get<TransformComponent, CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				mainCamera = &cameraComponent.Camera;
				transform = transformComponent;
				break;
			}
		}

		if (mainCamera)
		{			
			Renderer2D::BeginScene(*mainCamera, transform);
			m_Registry.view<TransformComponent, SpiriteRendererComponent>().each([&](entt::entity entity, const TransformComponent& transformComponent, const SpiriteRendererComponent& spirite) {
				Renderer2D::DrawQuad(transformComponent.GetTransform(), spirite.Color);
			});
			Renderer2D::EndScene();
		}
	}

	void Scene::RenderScene(TimeStamp ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		m_Registry.view<TransformComponent, SpiriteRendererComponent>().each([&](entt::entity entity, const TransformComponent& transformComponent, const SpiriteRendererComponent& spirite) {
			Renderer2D::DrawQuad(transformComponent.GetTransform(), spirite.Color);
			});
		Renderer2D::EndScene();
	}

	template<typename T>
	void Scene::OnEntityAddComponent(Entity* entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnEntityAddComponent<CameraComponent>(Entity* entity, CameraComponent& com)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			com.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	template<>
	void Scene::OnEntityAddComponent<TagComponent>(Entity* entity, TagComponent& com)
	{
		
	}

	template<>
	void Scene::OnEntityAddComponent<TransformComponent>(Entity* entity, TransformComponent& com)
	{

	}

	template<>
	void Scene::OnEntityAddComponent<SpiriteRendererComponent>(Entity* entity, SpiriteRendererComponent& com)
	{

	}

	template<>
	void Scene::OnEntityAddComponent<NativeScriptComponent>(Entity* entity, NativeScriptComponent& com)
	{
		com.Bind<QuadController>();
	}

	std::pair<glm::mat4, glm::mat4> Scene::GetMainCamera()
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity).GetTransform();
			if (cameraComponent.Primary)
			{
				return std::make_pair(cameraComponent.Camera.GetProjection(), transform);
			}
		}
		return {};
	}
}