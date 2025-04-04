#pragma once
#include "Components.h"
#include <entt/entt.hpp>
#include <iostream>
#include "Systems.h"

class PhysicsSystem : System {
public:
	virtual void Update(entt::registry& registry, float deltaTime) override {
		auto view = registry.view<TransformComponent, RigidBodyComponent>();
		for (auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			auto& rigidbody = view.get<RigidBodyComponent>(entity);

			rigidbody.Velocity += rigidbody.Acceleration * deltaTime;
			transform.Position += rigidbody.Velocity * deltaTime;
		}
	}
	~PhysicsSystem() {
		std::cout << "PhysicsSystem destroyed" << std::endl;
	}
};
