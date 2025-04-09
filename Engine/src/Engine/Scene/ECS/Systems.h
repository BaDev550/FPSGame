#pragma once
#include "Components.h"
#include "Engine/Window/Window.h"
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
		RenderSystem(std::shared_ptr<Shader>& shader, std::shared_ptr<Shader>& shadowShader, std::shared_ptr<Window>& window, entt::registry& registry)
			: _Shader(shader), _Window(window), _Registry(registry), _ShadowShader(shadowShader)
		{
			float aspectRatio = (float)_Window->GetWidth() / (float)_Window->GetHeight();
			auto view = _Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				camera.SetAspectRatio(aspectRatio);
			}
		}
	
		void Draw() {
			UpdateCameraSystem();

			glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));
			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
			glm::mat4 lightView = glm::lookAt(-lightDir * distance, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;
			_Shader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

			RenderShadowPass(lightSpaceMatrix);
			RenderMainScene();
		}

		void RenderShadowPass(const glm::mat4& lightSpaceMatrix) {
			_ShadowShader->Bind();

			for (auto entity : _Registry.view<TransformComponent, MeshComponent>()) {
				auto& transform = _Registry.get<TransformComponent>(entity);
				auto& mesh = _Registry.get<MeshComponent>(entity);

				if (mesh.ModelLoaded()) {
					mesh.DrawShadow(_ShadowShader, transform, lightSpaceMatrix);
				}
			}
		}

		void RenderMainScene() {
			_Shader->Bind();

			for (auto entity : _Registry.view<TransformComponent, MeshComponent>()) {
				auto& transform = _Registry.get<TransformComponent>(entity);
				auto& mesh = _Registry.get<MeshComponent>(entity);

				if (mesh.ModelLoaded()) {
					mesh.Draw(_Shader, transform);
				}
			}
		}

		void UpdateCameraSystem()
		{
			auto view = _Registry.view<CameraComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);

				transform.Position = camera.Position;

				transform.Rotation.y = camera.Yaw;
				transform.Rotation.x = camera.Pitch;
				_Shader->SetVec3("u_ViewPos", transform.Position);
				_Shader->SetVec3("u_LightPos", transform.Position);

				camera.UpdateCameraVectors();
			}
		}
	private:
		std::shared_ptr<Shader> _Shader;
		std::shared_ptr<Shader> _ShadowShader;
		std::shared_ptr<Window> _Window;
		entt::registry& _Registry;
		float distance = 60.0f;
	};
}