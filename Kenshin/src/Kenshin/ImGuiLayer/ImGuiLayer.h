#pragma once
#include "Kenshin/Core.h"
#include "Kenshin/Layer.h"

namespace Kenshin
{
	class KENSHIN_API ImGuiLayer:public Layer
	{
		public: 
			ImGuiLayer();
			~ImGuiLayer();
			void OnAttach() override;
			void OnDetach() override;
			void OnUpdate() override;
			void OnEvent(Event& e) override;
			void SetDarkThemecolors();
	};
}