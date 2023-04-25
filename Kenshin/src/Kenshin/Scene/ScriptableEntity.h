#pragma once
#include "Entity.h"
#include "Kenshin/Core/TimeStamp.h"

namespace Kenshin
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		~ScriptableEntity() = default;
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
		
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(TimeStamp ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}