#include "PlayerPawn.h"

void PlayerPawn::OnStart()
{
}

void PlayerPawn::OnUpdate()
{
	if (EngineInput::IsKeyPressed(E_KEY_W)) {
		GetComponent<Engine::TransformComponent>().Position += _FPCamera.Front * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_S)) {
		GetComponent<Engine::TransformComponent>().Position -= _FPCamera.Front * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_A)) {
		GetComponent<Engine::TransformComponent>().Position -= _FPCamera.Right * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_D)) {
		GetComponent<Engine::TransformComponent>().Position += _FPCamera.Right * 0.1f;
	}

}

void PlayerPawn::OnMouseMoved(Engine::MouseMovedEvent& e)
{
	if (!IsValid()) return;

	if (_bfirstPressed)
	{
		_LastX = e.GetX();
		_LastY = e.GetY();
		_bfirstPressed = false;
	}

	float xOffset = e.GetX() - _LastX;
	float yOffset = _LastY - e.GetY();

	_LastX = e.GetX();
	_LastY = e.GetY();

	_FPCamera.ProcessMouseMovement(xOffset, yOffset);
}
