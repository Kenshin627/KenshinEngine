#pragma once
#include "kspch.h"
#include "events/Event.h"
#include "Kenshin/Core/Timestep.h"

namespace Kenshin {
	class KS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRender() const {};
		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}