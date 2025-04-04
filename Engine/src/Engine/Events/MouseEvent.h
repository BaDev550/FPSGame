#pragma once

#include "Event.h"

namespace Engine
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: _MouseX(x), _MouseY(y) {}
	
		float GetX() const { return _MouseX; }
		float GetY() const { return _MouseY; }
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _MouseX << ", " << _MouseY;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float _MouseX, _MouseY;
	};
	
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: _XOffset(xOffset), _YOffset(yOffset) {}
	
		float GetXOffset() const { return _XOffset; }
		float GetYOffset() const { return _YOffset; }
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float _XOffset, _YOffset;
	};
	
	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return _Button; }
	
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const int button)
			: _Button(button) {}
	
		int _Button;
	};
	
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) {}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _Button;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) {}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _Button;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
