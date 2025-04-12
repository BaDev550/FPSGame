#pragma once

#include "IPawn.h"

#include "Engine/Scene/ECS/Entity.h"
#include "Engine/Scene/ECS/Components.h"

#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine {
	class ICharacter : public IPawn
	{
	public:
		ICharacter(std::string& name) : IPawn(name) {}
		virtual ~ICharacter() = default;

		virtual void OnMouseMoved(MouseMovedEvent& e) = 0;
		virtual void OnMouseButtonPressed(MouseButtonPressedEvent& e) = 0;
		virtual void OnKeyPressed(KeyPressedEvent& e) = 0;
		virtual void OnKeyReleased(KeyReleasedEvent& e) = 0;
	};
}