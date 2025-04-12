#pragma once

#include "Engine.h"

class FPCamera : public EngineActor
{
public:
	FPCamera() : EngineActor("FP Camera") {
		AddComponent<Engine::CameraComponent>(&GetComponent<Engine::TransformComponent>());
	}

	virtual void OnStart() override {

	}
	virtual void OnUpdate() override {
		GetComponent<Engine::TransformComponent>().Position = GetParent()->GetComponent<Engine::TransformComponent>().Position;
		GetParent()->GetComponent<Engine::TransformComponent>().Rotation.x = GetComponent<Engine::TransformComponent>().Rotation.x;
		GetParent()->GetComponent<Engine::TransformComponent>().Rotation.y = GetComponent<Engine::TransformComponent>().Rotation.y * -1.0f;
		GetParent()->GetComponent<Engine::TransformComponent>().Rotation.z = GetComponent<Engine::TransformComponent>().Rotation.z;
	}

	void ProcessMouseMovement(double x, double y){
		if (!IsValid()) return;

		if (_bfirstPressed)
		{
			_LastX = x;
			_LastY = y;
			_bfirstPressed = false;
		}

		float xOffset = x - _LastX;
		float yOffset = _LastY - y;

		_LastX = x;
		_LastY = y;

		GetCamera().ProcessMouseMovement(xOffset, yOffset);
	}

	Engine::CameraComponent GetCamera() { return GetComponent<Engine::CameraComponent>(); }
private:
	float _LastX = 0.0f, _LastY = 0.0f;
	bool _bfirstPressed = true;
};