#include "kspch.h"
#include "LayerStack.h"

namespace Kenshin
{
	LayerStack::LayerStack() 
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Layers)
		{
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
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
			m_Layers.erase(it); 
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverLay(Layer* overlay)
	{
		auto it = std::find(m_Layers.cbegin(), m_Layers.cend(), overlay);
		if (it != m_Layers.cend())
		{
			m_Layers.erase(it);
		}
	}
}