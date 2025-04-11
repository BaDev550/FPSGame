#pragma once

#include "Engine.h"

class PlayerPawn : public EnginePawn
{
public:
	PlayerPawn() : EnginePawn("Player Pawn") {}

	virtual bool OnMouseMoved(Engine::MouseMovedEvent& e) override {
		return false;
	}
	virtual bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e) override {
		return false;
	}
	virtual bool OnKeyPressed(Engine::KeyPressedEvent& e) override {
		std::cout << e.ToString() << std::endl;
		return false;
	}
	virtual bool OnKeyReleased(Engine::KeyReleasedEvent& e) override {
		return false;
	}
};

