#pragma once
#include "kspch.h"
#include "events/Event.h"

namespace Kenshin {
	class KS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}
		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}