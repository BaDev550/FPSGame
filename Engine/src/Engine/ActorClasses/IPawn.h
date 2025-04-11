#pragma once

#include "IActor.h"

#include "Engine/Scene/ECS/Entity.h"
#include "Engine/Scene/ECS/Components.h"

#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine {
	class IPawn : public IActor
	{
	public:
		IPawn(std::string name) : IActor(name) {}
		virtual ~IPawn() = default;

		virtual bool OnMouseMoved(MouseMovedEvent& e) = 0;
		virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) = 0;
		virtual bool OnKeyPressed(KeyPressedEvent& e) = 0;
		virtual bool OnKeyReleased(KeyReleasedEvent& e) = 0;
	};
}