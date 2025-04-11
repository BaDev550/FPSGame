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
	public:
		std::unique_ptr<Model> _Model;
		bool bVisible = true;
		bool bDrawShadows = true;

		MeshComponent(const std::string& modelPath)
		{
			_Model = std::make_unique<Model>(modelPath);
		}
		bool ModelLoaded() { return _Model.get(); }

		void LoadModel(const std::string& modelPath) {
			_Model.reset();
			_Model = std::make_unique<Model>(modelPath);
		}
		std::string& GetModelPath() { return _Model->GetPath(); }
		std::vector<std::shared_ptr<Material>> GetMaterials() { return _Model->GetAllMaterials(); }
		void SetMaterials(std::vector<std::shared_ptr<Material>>& materials) { _Model->SetMaterial(materials); }
		void SetVisiblity(bool Visible) { bVisible = Visible; }
		void SetDrawShadows(bool Draw) { bDrawShadows = Draw; }
		void Draw(std::shared_ptr<Shader> shader, TransformComponent& transform) {
			_Model->Draw(shader, transform.GetModelMatrix());
		}
		void DrawShadow(std::shared_ptr<Shader> ShadowShader, TransformComponent& transform, glm::mat4 LightSpaceMatrix) {
			_Model->DrawShadow(ShadowShader, transform.GetModelMatrix(), LightSpaceMatrix);
		}
	};

	enum class ECameraDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	struct CameraComponent
	{
		CameraComponent(){
			UpdateCameraVectors();
		}

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

		float Yaw = -90.0f;
		float Pitch = 0.0f;

		float MovementSpeed = 5.0f;
		float MouseSensitivity = 0.1f;
		float Zoom = 45.0f;

		float AspectRatio = 1.778f;

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		glm::mat4 GetProjectionMatrix(float nearPlane, float farPlane)
		{
			return glm::perspective(glm::radians(Zoom), AspectRatio, nearPlane, farPlane);
		}

		glm::mat4 GetViewProjectionMatrix()
		{
			return GetProjectionMatrix(0.1f, 100.0f) * GetViewMatrix();
		}

		void ProcessKeyboard(ECameraDirection direction, float deltaTime) {
			float velocity = MovementSpeed * deltaTime;
			if (direction == ECameraDirection::FORWARD)
				Position += Front * velocity;
			if (direction == ECameraDirection::BACKWARD)
				Position -= Front * velocity;
			if (direction == ECameraDirection::LEFT)
				Position -= Right * velocity;
			if (direction == ECameraDirection::RIGHT)
				Position += Right * velocity;
		}

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			UpdateCameraVectors();
		}

		void ProcessMouseScroll(float yoffset) {
			Zoom -= yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 45.0f)
				Zoom = 45.0f;
		}
		void SetAspectRatio(float ar) { AspectRatio = ar; }
		void UpdateCameraVectors() {
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);

			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}

		inline glm::vec3 GetPosition() const { return Position; }
		inline glm::vec3 GetFront() const { return Front; }
		inline glm::vec3 GetUp() const { return Up; }
		inline glm::vec3 GetRight() const { return Right; }
	};
}