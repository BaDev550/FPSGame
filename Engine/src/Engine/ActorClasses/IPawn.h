#pragma once

#include "IActor.h"

#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine {
	class IPawn : public IActor
	{
	public:
		IPawn(std::string name) : IActor(name) {
			AddComponent<PawnComponent>(this);
			AddComponent<BoxColliderComponent>();
			PrintComponents();
		}
		virtual ~IPawn() = default;

		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;

		virtual void OnMouseMoved(MouseMovedEvent& e) {}
		virtual void OnMouseButtonPressed(MouseButtonPressedEvent& e) {}
		virtual void OnKeyPressed(KeyPressedEvent& e) {}
		virtual void OnKeyReleased(KeyReleasedEvent& e) {}
	};
}