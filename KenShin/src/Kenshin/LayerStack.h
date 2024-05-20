#pragma once
#include "kspch.h"
#include "Layer.h"

namespace Kenshin {
	class LayerStack
	{
		using StackType = std::vector<Layer*>;
		using StackIteratorType = StackType::iterator;
	public:
		LayerStack();
		~LayerStack();
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* lay);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* lay);
		inline StackIteratorType begin() { return m_Layers.begin(); }
		inline StackIteratorType end() { return m_Layers.end(); }
	private:
		StackType m_Layers;
		StackIteratorType m_Insert;
	};
}