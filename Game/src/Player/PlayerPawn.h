#pragma once

#include "Engine.h"
#include "FPCamera.h"

class PlayerPawn : public EnginePawn
{
public:
	PlayerPawn() : EnginePawn("Player Pawn") {
		AddComponent<EngineStaticMeshComponent>("Assets/Models/mario_2/mario_2.obj");

		_FPCamera = std::make_unique<FPCamera>();
		AddChild(_FPCamera.get());
	}

	virtual void OnStart() override;
	virtual void OnUpdate() override;

	virtual void OnMouseMoved(Engine::MouseMovedEvent& e) override;
	std::unique_ptr<FPCamera>& GetCameraEntity() { return _FPCamera; }
private:
	std::unique_ptr<FPCamera> _FPCamera;
};