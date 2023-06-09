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
		
	}

	Scene::~Scene() 
	{
		delete m_PhysicsWorld;
	}

	void Scene::RenderScene(const glm::mat4& viewpProjection)
	{
		Renderer2D::BeginScene(viewpProjection);
		m_Registry.view<TransformComponent, SpiriteRendererComponent>().each([&](entt::entity entity, const TransformComponent& transformComponent, const SpiriteRendererComponent& spirite) {
			Renderer2D::DrawSpirite(transformComponent.GetTransform(), spirite, (int)entity);
			});

		m_Registry.view<TransformComponent, CircleRendererComponent>().each([&](entt::entity entity, const TransformComponent& transformComponent, const CircleRendererComponent& circle) {
			Renderer2D::DrawCircle(transformComponent.GetTransform(), circle.Color, circle.Thinness, circle.Fade, (int)entity);
			});
		Renderer2D::EndScene();
	}

	void Scene::PhysicsUpdate(TimeStamp ts)
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

			b2Body* runtimeBody = (b2Body*)rb2d.RuntimeBody;
			auto& position = runtimeBody->GetPosition();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation.z = runtimeBody->GetAngle();
		}
	}

	void Scene::ScriptsUpdate(TimeStamp ts)
	{
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
	}

	template<typename...Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]() {
			if (src.HasComponent<Component>())
			{
				dst.AddOrReplace<Component>(src.GetComponent<Component>());
			}
		}(), ...);
	}

	template<typename...Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{		
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
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
		//Scripts update
		ScriptsUpdate(ts);

		//Physcis update
		PhysicsUpdate(ts);

		//Scene update
		auto cameraEntity = GetPrimaryCamera();
		if (cameraEntity)
		{
			RenderScene(cameraEntity.GetComponent<CameraComponent>().Camera.GetProjection() * cameraEntity.GetComponent<TransformComponent>().GetTransform());
		}
	}

	void Scene::OnUpdateEditor(const EditorCamera& camera)
	{
		RenderScene(camera.GetViewProjectionMatrix());
	}

	void Scene::OnUpdateSimulation(TimeStamp ts, const EditorCamera& camera)
	{
		//Physcis
		PhysicsUpdate(ts);
		//Scene
		RenderScene(camera.GetViewProjectionMatrix());
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
	void Scene::OnEntityAddComponent<IDComponent>(Entity* entity, IDComponent& com)
	{

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
	void Scene::OnEntityAddComponent<CircleRendererComponent>(Entity* entity, CircleRendererComponent& com)
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

	template<>
	void Scene::OnEntityAddComponent<CircleCollider2DComponent>(Entity* entity, CircleCollider2DComponent& com)
	{

	}

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				return { entity, this };
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

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
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
			bodyDef.angularDamping = rb2d.Damping;
			bodyDef.gravityScale = rb2d.GravityScale;
			bodyDef.bullet = rb2d.CCD;
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

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = cc2d.Radius * transform.Scale.x;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				fixtureDef.density = cc2d.Density;
				b2Fixture* b2fixture = b2body->CreateFixture(&fixtureDef);
				cc2d.RuntimeFixture = b2fixture;
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	Ref<Scene> Scene::Copy(const Ref<Scene>& other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto i = idView.rbegin(); i != idView.rend(); i++)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(*i).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(*i).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());

		CopyComponentIfExists(AllComponents{}, newEntity, entity);

		return newEntity;
	}
}