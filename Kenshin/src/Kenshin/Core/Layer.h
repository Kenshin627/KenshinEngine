#pragma once
#include "Core.h"
#include "../Events/Event.h"
#include "../Core/TimeStamp.h"

namespace Kenshin
{
	class KENSHIN_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual~Layer();
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(TimeStamp ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& e);
		std::string GetName() const { return m_DebugName; };
	private:
		std::string m_DebugName;
	};
}
