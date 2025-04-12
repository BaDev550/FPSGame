#pragma once

#include "Engine.h"

class PlayerPawn : public EnginePawn
{
public:
	PlayerPawn() : EnginePawn("Player Pawn") {
		AddComponent<EngineStaticMeshComponent>("Assets/Models/mario_2/mario_2.obj");
		AddComponent<Engine::CameraComponent>();
		_FPCamera = GetComponent<Engine::CameraComponent>();
	}

	virtual void OnStart() override;
	virtual void OnUpdate() override;

	virtual void OnMouseMoved(Engine::MouseMovedEvent& e) override;

	Engine::CameraComponent* GetCamera() { return &_FPCamera; }
private:
	Engine::CameraComponent _FPCamera;

	float _LastX = 0.0f, _LastY = 0.0f;
	bool _bfirstPressed = true;
};