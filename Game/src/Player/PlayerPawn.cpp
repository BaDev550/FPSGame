#include "PlayerPawn.h"

void PlayerPawn::OnStart()
{
	_FPCamera->GetComponent<Engine::TransformComponent>().SetLocalPosition({ 0.0f, 1.0f, 0.0f });
	GetComponent<Engine::BoxColliderComponent>().Size = glm::vec3(2.0f, 2.0f, 2.0f);
}

void PlayerPawn::OnUpdate()
{
	if (EngineInput::IsKeyPressed(E_KEY_W)) {
		GetComponent<Engine::TransformComponent>().Position += _FPCamera->GetCamera().GetFront() * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_S)) {
		GetComponent<Engine::TransformComponent>().Position -= _FPCamera->GetCamera().GetFront() * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_A)) {
		GetComponent<Engine::TransformComponent>().Position -= _FPCamera->GetCamera().GetRight() * 0.1f;
	}
	if (EngineInput::IsKeyPressed(E_KEY_D)) {
		GetComponent<Engine::TransformComponent>().Position += _FPCamera->GetCamera().GetRight() * 0.1f;
	}
}

void PlayerPawn::OnMouseMoved(Engine::MouseMovedEvent& e)
{
	_FPCamera->ProcessMouseMovement(e.GetX(), e.GetY());
}
