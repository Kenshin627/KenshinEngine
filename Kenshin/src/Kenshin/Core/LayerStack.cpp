#include "kspch.h"
#include "LayerStack.h"

namespace Kenshin
{
	LayerStack::LayerStack():m_LayerInsertIndex(0) { }

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverLay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.cbegin(), m_Layers.cend(), layer
		);
		if (it != m_Layers.cend())
		{
			layer->OnDetach();
			m_Layers.erase(it); 
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverLay(Layer* overlay)
	{
		auto it = std::find(m_Layers.cbegin(), m_Layers.cend(), overlay);
		if (it != m_Layers.cend())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}