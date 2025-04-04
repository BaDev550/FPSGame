#pragma once
#include "Engine.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale = glm::vec3(1.0f);
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
	void Draw(std::shared_ptr<EngineShader> shader) {
		Model->Draw(shader);
	}
};