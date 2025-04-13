#include "PlayerPawn.h"

void PlayerPawn::OnStart()
{
	ColliderEvents();
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

void PlayerPawn::ColliderEvents()
{
	collider.OnCollisionBeginCallback = [](const Engine::OnCollisionBegin& e) {
		std::cout << "Collision with entity ID: " << e.GetOtherActor().GetComponent<Engine::NameComponent>().name << "\n";
	};

	collider.OnCollisionEndCallback = [](const Engine::OnCollisionEnd& e) {
		std::cout << "Stopped colliding with entity ID: " << e.GetOtherActor().GetComponent<Engine::NameComponent>().name << "\n";
	};
}
