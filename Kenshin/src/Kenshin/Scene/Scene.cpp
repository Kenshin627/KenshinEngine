#include "kspch.h"
#include "Scene.h"
#include "Kenshin/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "Kenshin/Scene/CameraController.h"
#include "Kenshin/Scene/QuadController.h"

#include "include/box2d/b2_world.h"
#include "include/box2d/b2_body.h"
#include "include/box2d/b2_fixture.h"
#include "include/box2d/b2_polygon_shape.h"
#include "include/box2d/b2_circle_shape.h"
#include "Kenshin/Physics/Physics2D.h"

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

	void Scene::OnUpdateRuntime(TimeStamp ts)
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

		//Physcis
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, 6, 2);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto& e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				auto runtimeBody = (b2Body*)rb2d.RuntimeBody;
				transform.Translation.x = runtimeBody->GetPosition().x;
				transform.Translation.y = runtimeBody->GetPosition().y;
				transform.Rotation.z = runtimeBody->GetAngle();
			}
		}

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
				Renderer2D::DrawSpirite(transformComponent.GetTransform(), spirite, (int)entity);
			});
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(TimeStamp ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		m_Registry.view<TransformComponent, SpiriteRendererComponent>().each([&](entt::entity entity, const TransformComponent& transformComponent, const SpiriteRendererComponent& spirite) {
			Renderer2D::DrawSpirite(transformComponent.GetTransform(), spirite, (int)entity);
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

	template<>
	void Scene::OnEntityAddComponent<Rigidbody2DComponent>(Entity* entity, Rigidbody2DComponent& com)
	{
		
	}

	template<>
	void Scene::OnEntityAddComponent<BoxCollider2DComponent>(Entity* entity, BoxCollider2DComponent& com)
	{
		
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

	void Scene::OnRuntimeStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnRuntimeStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto& e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.x = transform.Translation.x;
			bodyDef.position.y = transform.Translation.y;
			bodyDef.angle = transform.Rotation.z;
			bodyDef.fixedRotation = rb2d.FixedRotation;

			b2Body* b2body = m_PhysicsWorld->CreateBody(&bodyDef);
			rb2d.RuntimeBody = b2body;
			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				fixtureDef.density = bc2d.Density;
				b2Fixture* b2fixture = b2body->CreateFixture(&fixtureDef);
				bc2d.RuntimeFixture = b2fixture;
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{

	}
}