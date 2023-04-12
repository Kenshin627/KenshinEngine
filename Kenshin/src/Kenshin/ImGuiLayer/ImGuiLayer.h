#pragma once
#include "Kenshin/Core.h"
#include "Kenshin/Layer.h"

namespace Kenshin
{
	class ImGuiLayer:public Layer
	{
		/*void Layer::OnAttach() {}
		void Layer::OnDetach() {}
		void Layer::OnUpdate() {}
		void Layer::OnEvent(Event& e) {}*/
		public: 
			ImGuiLayer();
			~ImGuiLayer();
			void OnAttach() override;
			void OnDetach() override;
			void OnUpdate() override;
			void OnEvent(Event& e) override;
	};
}