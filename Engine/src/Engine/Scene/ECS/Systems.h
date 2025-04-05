#pragma once
#include "Components.h"
#include "entt/entt.hpp"

namespace Engine
{
	class PhysicsSystem{
	public:
		void Update(entt::registry& registry, float deltaTime) {
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
	
	class RenderSystem {
	public:
		RenderSystem(std::shared_ptr<Shader>& shader)
			: _Shader(shader)
		{}
	
		void Draw(entt::registry& registry) {
			auto view = registry.view<TransformComponent, MeshComponent>();
			for (auto entity : view) {
				auto& transform = view.get<TransformComponent>(entity);
				auto& mesh = view.get<MeshComponent>(entity);
	
				if (mesh.ModelLoaded())
					mesh.Draw(_Shader, transform);
			}
		}
	private:
		std::shared_ptr<Shader> _Shader;
	};
}