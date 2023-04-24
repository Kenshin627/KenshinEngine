#include "kspch.h"
#include "Entity.h"

namespace Kenshin
{
	Entity::Entity(entt::entity id, Scene* scene, const std::string& name) : m_EntityHandle(id), m_Scene(scene){ }
}