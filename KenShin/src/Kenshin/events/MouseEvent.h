#pragma once
#include "Event.h"
#include <sstream>

namespace Kenshin
{
	class KS_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}
		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }
		std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_MouseX, m_MouseY;
	};

	class KS_API MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float offsetX, float offsetY) :m_OffsetX(offsetX), m_OffsetY(offsetY) {}
		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class KS_API MouseButtonEvent :public Event
	{
	public:
		inline int GetButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		MouseButtonEvent(int button): m_Button(button) {  }
		int m_Button;

	};

	class KS_API MousePressedEvent :public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button) :MouseButtonEvent(button) {}
		EVENT_CLASS_TYPE(MouseButtonPressed)
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressed Event: " << m_Button;
			return ss.str();
		}
	};

	class KS_API MouseReleaseEvent :public MouseButtonEvent
	{
	public:
		MouseReleaseEvent(int button) :MouseButtonEvent(button) {}
		EVENT_CLASS_TYPE(MouseButtonReleased)
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseREleaseEvent: " << m_Button;
			return ss.str();
		}
	};
}