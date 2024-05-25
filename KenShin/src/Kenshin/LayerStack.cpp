#include "kspch.h"
#include "LayerStack.h"

namespace Kenshin {
	LayerStack::LayerStack()
	{
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
		m_Layers.emplace(m_Layers.cbegin() + m_InsertIndex, layer);
		m_InsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverLay(Layer* lay)
	{
		m_Layers.push_back(lay);
		lay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto& it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_InsertIndex--;
			(*it)->OnDetach();
		}
	}

	void LayerStack::PopOverLayer(Layer* lay)
	{
		const auto& it = std::find(m_Layers.begin(), m_Layers.end(), lay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			(*it)->OnDetach();
		}
	}
}