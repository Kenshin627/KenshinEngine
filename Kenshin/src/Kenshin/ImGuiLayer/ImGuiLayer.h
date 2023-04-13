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
			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnEvent(Event& e) override;
			void SetDarkThemecolors();
			void Begin();
			void End();
			void BlockEvents(bool block) { m_BlockEvents = block; };
			uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
	};
}