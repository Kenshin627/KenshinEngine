#pragma once
#include"include/box2d/b2_body.h"
#include "Kenshin/Scene/Components.h"

namespace Kenshin
{
	namespace Utils 
	{
		inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
		{
			switch (bodyType)
			{
			case Rigidbody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
			}
			KS_CORE_ASSERT(false, "Unknown body type");
			return b2_staticBody;
		}
	}
}