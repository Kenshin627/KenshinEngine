#include "kspch.h"
#include "SceneSerializer.h"
#include "Kenshin/Scene/Entity.h"
#include "Kenshin/Core/UUID.h"
#include "Kenshin/Scene/SceneCamera.h"
#include <yaml-cpp/yaml.h>
#include <glm.hpp>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Kenshin::UUID>
	{
		static Node encode(const Kenshin::UUID& rhs)
		{
			Node node;
			node.push_back((uint64_t)rhs);
			return node;
		}

		static bool decode(const Node& node, Kenshin::UUID& rhs)
		{
			rhs = node.as<uint64_t>();
			return true;
		}
	};
}

namespace Kenshin
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v2)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v2.x << v2.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v3.x << v3.y << v3.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v4.x << v4.y << v4.z << v4.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		KS_CORE_ASSERT(entity.HasComponent<IDComponent>(), "");
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity.GetName();
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::Key << "PerpectiveVerticalFOV" << YAML::Value <<
				camera.GetPerpectiveVerticalFov();
			out << YAML::Key << "PerpectiveNear" << YAML::Value <<
				camera.GetPerspectiveNear();
			out << YAML::Key << "PerpectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::EndMap;
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpiriteRendererComponent>())
		{
			out << YAML::Key << "SpiriteRendererComponent";
			out << YAML::BeginMap;

			auto& sc = entity.GetComponent<SpiriteRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			if (sc.Texture)
			{
				out << YAML::Key << "TexturePath" << YAML::Value << sc.Texture->GetPath();
			}

			out << YAML::Key << "TilingFactor" << YAML::Value << sc.TilingFactor;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rc = entity.GetComponent<Rigidbody2DComponent>();

			out << YAML::Key << "BodyType" << YAML::Value << (int)rc.Type;
			out << YAML::Key << "Damping" << YAML::Value << rc.Damping;
			out << YAML::Key << "GravityScale" << YAML::Value << rc.GravityScale;
			out << YAML::Key << "FixedRotation" << YAML::Value << rc.FixedRotation;
			out << YAML::Key << "CCD" << YAML::Value << rc.CCD;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) 
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "Offset" << YAML::Value << bc.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc.Size;
			out << YAML::Key << "Density" << YAML::Value << bc.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc.RestitutionThreshold;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :m_Scene(scene) {}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
			{
				return;
			}
			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneSerializer::DeSerialize(const std::string& filePath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath);
		}
		catch (YAML::ParserException e)
		{
			KS_CORE_ERROR("Failed to load .kenshin file '{0}' \n   {1}", filePath, e.what());
			return false;
		}
		if (!data["Scene"])
		{
			return false;

		}

		std::string sceneName = data["Scene"].as<std::string>();
		KS_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities) 
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}
				KS_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				//transform
				auto transform = entity["TransformComponent"];
				if (transform)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transform["Translation"].as<glm::vec3>();
					tc.Rotation    = transform["Rotation"].as<glm::vec3>();
					tc.Scale       = transform["Scale"].as<glm::vec3>();
				}

				//CameraComponent
				auto cameraEntity = entity["CameraComponent"];
				if (cameraEntity)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProps = cameraEntity["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Camera.SetPerpectiveVerticalFov(cameraProps["PerpectiveVerticalFOV"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraProps["PerpectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProps["PerpectiveFar"].as<float>());

					cc.Primary = cameraEntity["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraEntity["FixedAspectRatio"].as<bool>();
				}

				//SpiriteRendererComponent
				auto sprite = entity["SpiriteRendererComponent"];
				if (sprite)
				{
					auto& sc = deserializedEntity.AddComponent<SpiriteRendererComponent>();
					sc.Color = sprite["Color"].as<glm::vec4>();
					auto texturePath = sprite["TexturePath"];
					if (texturePath)
					{
						sc.Texture = Texture2D::Create(texturePath.as<std::string>());
					}
					sc.TilingFactor = sprite["TilingFactor"].as<float>();				
				}

				//Rigidbody2DComponent
				auto rigidBody = entity["Rigidbody2DComponent"];
				if (rigidBody)
				{
					auto& rc = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rc.Type = (Rigidbody2DComponent::BodyType)rigidBody["BodyType"].as<int>();
					rc.Damping = rigidBody["Damping"].as<float>();
					rc.GravityScale = rigidBody["GravityScale"].as<float>();
					rc.FixedRotation = rigidBody["FixedRotation"].as<bool>();
					rc.CCD = rigidBody["CCD"].as<bool>();
				}

				//Box2DCollider
				auto box2d = entity["BoxCollider2DComponent"];
				if (box2d)
				{
					auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc.Offset = box2d["Offset"].as<glm::vec2>();
					bc.Size = box2d["Size"].as<glm::vec2>();
					bc.Density = box2d["Density"].as<float>();
					bc.Friction = box2d["Friction"].as<float>();
					bc.Restitution = box2d["Restitution"].as<float>();
					bc.RestitutionThreshold = box2d["RestitutionThreshold"].as<float>();
				}
			}
		}
		return true;
	}
}