#pragma once
#include "Engine.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale = glm::vec3(1.0f);

	glm::mat4& GetModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = glm::scale(model, Scale);
		model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
		return model;
	}
};

struct RigidBodyComponent {
	glm::vec3 Velocity;
	glm::vec3 Acceleration;
	float Mass = 1.0f;
};

struct MeshComponent {
	std::unique_ptr<EngineModel> Model;

	MeshComponent(const std::string& modelPath) {
		Model = EngineLoadStaticMesh(modelPath);
	}
	void Draw(std::shared_ptr<EngineShader> shader, TransformComponent& transform) {
		Model->Draw(shader, transform.GetModelMatrix());
	}
};