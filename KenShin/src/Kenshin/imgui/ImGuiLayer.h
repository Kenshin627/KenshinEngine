#pragma once
#include "Kenshin/Core.h"
#include "Kenshin/Layer.h"
#include "Kenshin/events/ApplicationEvent.h"
#include "Kenshin/events/KeyEvent.h"
#include "Kenshin/events/MouseEvent.h"

enum ImGuiKey;

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
		ImGuiKey GlfwToImGuiKeyMap(int key) const;
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleaseEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnMouseMovedEvent(MouseMoveEvent& e);
		bool OnMouseScrolledEvent(MouseScrollEvent& e);
		bool OnMousePressedEvent(MousePressedEvent& e);
		bool OnMouseReleasedEvent(MouseReleaseEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
	};
}