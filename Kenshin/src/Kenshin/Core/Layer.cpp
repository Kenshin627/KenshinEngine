#include "kspch.h"
#include "Layer.h"

namespace Kenshin
{
	Layer::Layer(const std::string& name) :m_DebugName(name)
	{}

	Layer::~Layer() {}

	void Layer::OnAttach() {}
	void Layer::OnDetach() {}
	void Layer::OnUpdate(TimeStamp ts) {}
	void Layer::OnEvent(Event& e) {}
	void Layer::OnImGuiRender(TimeStamp ts) {}
}
