#pragma once
#include "Kenshin/Core/Core.h"
#include "Kenshin/Core/Layer.h"

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
			virtual void OnImGuiRender() override;
			void SetDarkThemecolors();
			void Begin();
			void End();
			void BlockEvents(bool block) { m_BlockEvents = block; };
			uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
		bool m_ShowDemoWindow = true;
	};
}