#include "kspch.h"
#include "LayerStack.h"

namespace Kenshin {
	LayerStack::LayerStack()
	{
		m_Insert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* l : m_Layers)
		{
			delete l;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Insert = m_Layers.emplace(m_Insert, layer);
	}

	void LayerStack::PushOverLay(Layer* lay)
	{
		m_Layers.push_back(lay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto& it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_Insert--;
		}
	}

	void LayerStack::PopOverLayer(Layer* lay)
	{
		const auto& it = std::find(m_Layers.begin(), m_Layers.end(), lay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}
}