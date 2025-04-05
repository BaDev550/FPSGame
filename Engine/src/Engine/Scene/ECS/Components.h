#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Renderer/Model.h"

namespace Engine
{
	struct NameComponent {
		std::string name;
	
		NameComponent() = default;
		NameComponent(const std::string& name) : name(name) {}
	};
	
	struct TransformComponent {
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);
	
		glm::mat4& GetModelMatrix() const {
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, Position);
			modelMatrix = glm::scale(modelMatrix, Scale);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
			return modelMatrix;
		}
	};
	
	struct RigidBodyComponent {
		glm::vec3 Velocity;
		glm::vec3 Acceleration;
		float Mass = 1.0f;
	};
	
	struct MeshComponent {
	private:
		std::unique_ptr<Model> _Model;
	
	public:
		MeshComponent(const std::string& modelPath) {
			_Model = std::make_unique<Model>(modelPath);
		}
		bool ModelLoaded() { return _Model.get(); }
		void Draw(std::shared_ptr<Shader> shader, TransformComponent& transform) {
			_Model->Draw(shader, transform.GetModelMatrix());
		}
	};
}