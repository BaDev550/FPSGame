#include "PlayerPawn.h"

void PlayerPawn::OnStart()
{
	collider.OnCollisionBeginCallback = BIND_EVENT_FUNCTION(PlayerPawn::OnCollisionBegin);
	collider.OnCollisionEndCallback = BIND_EVENT_FUNCTION(PlayerPawn::OnCollisionEnded);
	std::cout << "Callback set: " << (bool)collider.OnCollisionBeginCallback << std::endl;
}

void PlayerPawn::OnUpdate()
{
	if(HasComponent<Engine::RigidBodyComponent>()){
		auto& rb = GetComponent<Engine::RigidBodyComponent>();

		const float movementForce = 50.0f;
		const float maxSpeed = 5.0f;

		glm::vec3 front = _FPCamera->GetCamera().GetFront();
		glm::vec3 right = _FPCamera->GetCamera().GetRight();
		front.y = 0.0f;
		right.y = 0.0f;
		front = glm::normalize(front);
		right = glm::normalize(right);

		glm::vec3 moveDir(0.0f);
		if (EngineInput::IsKeyPressed(E_KEY_W)) moveDir += front;
		if (EngineInput::IsKeyPressed(E_KEY_S)) moveDir -= front;
		if (EngineInput::IsKeyPressed(E_KEY_A)) moveDir -= right;
		if (EngineInput::IsKeyPressed(E_KEY_D)) moveDir += right;

		if (glm::length(moveDir) > 0.1f) {
			moveDir = glm::normalize(moveDir);

			glm::vec3 desiredVelocity = moveDir * maxSpeed;
			desiredVelocity.y = rb.Velocity.y;

			rb.AddForce((desiredVelocity - rb.Velocity) * rb.Mass);
		}

		if (EngineInput::IsKeyPressed(E_KEY_SPACE)) {
			rb.AddForce({ 0.0f, 10.0f * rb.Mass, 0.0f });
		}
	}
}

void PlayerPawn::OnMouseMoved(Engine::MouseMovedEvent& e)
{
	_FPCamera->ProcessMouseMovement(e.GetX(), e.GetY());
}

bool PlayerPawn::OnCollisionBegin(Engine::OnCollisionBegin& e)
{
	std::cout << "Collided Entity: " << e.GetOtherActor().GetComponent<Engine::NameComponent>().name << std::endl;
	return true;
}

bool PlayerPawn::OnCollisionEnded(Engine::OnCollisionEnd& e)
{
	return true;
}
