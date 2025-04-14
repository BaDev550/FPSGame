#pragma once
#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Renderer/Model.h"

#include "Jolt/Jolt.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Jolt/RegisterTypes.h"
#include <functional>

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
	
		glm::vec3 LocalPosition = glm::vec3(0.0f);
		glm::vec3 LocalRotation = glm::vec3(0.0f);
		glm::vec3 LocalScale = glm::vec3(0.0f);

		glm::mat4& GetModelMatrix() const {
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, Position + LocalPosition);
			modelMatrix = glm::scale(modelMatrix, Scale + LocalScale);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.x + LocalRotation.x), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.y + LocalRotation.y), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.z + LocalRotation.z), glm::vec3(0, 0, 1));
			return modelMatrix;
		}

		void SetPosition(glm::vec3 pos) { Position = pos; }
		void SetRotation(glm::vec3 rot) { Rotation = rot; }
		void SetScale(glm::vec3 scale) { Scale = scale; }

		void SetLocalPosition(glm::vec3 lpos) { LocalPosition = lpos; }
		void SetLocalRotation(glm::vec3 lrot) { LocalRotation = lrot; }
		void SetLocalScale(glm::vec3 lscale) { LocalScale = lscale; }
	};
	
	struct RigidBodyComponent {
		JPH::BodyID BodyID;
		float Mass = 1.0f;
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
		glm::vec3 PendingForce = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ConstantForce = { 0.0f, 0.0f, 0.0f };
		float LinearDamping = 0.1f;
		float AngularDamping = 0.1f;
		bool IsKinematic = false;

		void AddForce(const glm::vec3& force) {
			PendingForce += force;
		}

		void SetVelocity(const glm::vec3& velocity) {
			Velocity = velocity;
		}

		void ClearForces() {
			PendingForce = { 0.0f, 0.0f, 0.0f };
			ConstantForce = { 0.0f, 0.0f, 0.0f };
		}
	};

	enum class ECollisionMode
	{
		Disabled,
		Enabled
	};
	class OnCollisionBegin;
	class OnCollisionEnd;
	struct BoxColliderComponent
	{
		glm::vec3 Size = { 1.0f, 1.0f, 1.0f };
		bool IsDynamic = true;
		ECollisionMode Mode = ECollisionMode::Enabled;

		std::function<bool(OnCollisionBegin&)> OnCollisionBeginCallback;
		std::function<bool(OnCollisionEnd&)> OnCollisionEndCallback;

		std::unordered_set<uint32_t> CollidingWith;

		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent&) = default;
		BoxColliderComponent& operator=(const BoxColliderComponent&) = default;
		BoxColliderComponent(BoxColliderComponent&&) = default;
		BoxColliderComponent& operator=(BoxColliderComponent&&) = default;
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
		CameraComponent(TransformComponent* transform) : Transform(transform) {
			UpdateCameraVectors();
		}

	private:
		TransformComponent* Transform;
		glm::vec3 Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };
	public:
		float MovementSpeed = 5.0f;
		float MouseSensitivity = 0.1f;
		float Zoom = 45.0f;

		float AspectRatio = 1.778f;

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt((Transform->Position + Transform->LocalPosition), (Transform->Position + Transform->LocalPosition) + Front, Up);
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
				Transform->Position += Front * velocity;
			if (direction == ECameraDirection::BACKWARD)
				Transform->Position -= Front * velocity;
			if (direction == ECameraDirection::LEFT)
				Transform->Position -= Right * velocity;
			if (direction == ECameraDirection::RIGHT)
				Transform->Position += Right * velocity;
		}

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Transform->Rotation.y += xoffset;
			Transform->Rotation.z += yoffset;

			if (constrainPitch)
			{
				if (Transform->Rotation.z > 89.0f)
					Transform->Rotation.z = 89.0f;
				if (Transform->Rotation.z < -89.0f)
					Transform->Rotation.z = -89.0f;
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
			front.x = cos(glm::radians(Transform->Rotation.y)) * cos(glm::radians(Transform->Rotation.z));
			front.y = sin(glm::radians(Transform->Rotation.z));
			front.z = sin(glm::radians(Transform->Rotation.y)) * cos(glm::radians(Transform->Rotation.z));
			Front = glm::normalize(front);

			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}

		inline glm::vec3 GetPosition() const { return Transform->Position; }
		inline glm::vec3 GetFront() const { return Front; }
		inline glm::vec3 GetUp() const { return Up; }
		inline glm::vec3 GetRight() const { return Right; }
	};

	class IPawn;
	struct PawnComponent {
	public:
		PawnComponent(IPawn* inst) : instance(inst) {}
		IPawn* GetPawn() const { return instance; }
	private:
		IPawn* instance = nullptr;
	};

	class IActor;
	struct ActorComponent {
	public:
		ActorComponent(IActor* inst) : instance(inst) {}
		IActor* GetActor() const { return instance; }
	private:
		IActor* instance = nullptr;
	};
}