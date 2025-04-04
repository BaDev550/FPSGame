#pragma once

#include "Event.h"

namespace Engine
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return _KeyCode; }
	
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const int keycode)
			: _KeyCode(keycode) {}
	
		int _KeyCode;
	};
	
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keycode, int repeatCount)
			: KeyEvent(keycode), _RepeatCount(repeatCount) {}
	
		inline int IsRepeat() const { return _RepeatCount; }
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _KeyCode << " (repeat = " << _RepeatCount << ")";
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int _RepeatCount;
	};
	
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _KeyCode;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(KeyReleased)
	};
	
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << _KeyCode << std::endl;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(KeyTyped)
	};
}
