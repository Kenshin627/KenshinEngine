#pragma once
#include "Kenshin/Core.h"
#include "Kenshin/Layer.h"

namespace Kenshin {
	class KS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& name);
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
	};
}