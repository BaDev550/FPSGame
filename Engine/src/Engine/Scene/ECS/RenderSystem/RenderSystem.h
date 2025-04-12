#pragma once
#include "Engine/Scene/ECS/Components.h"
#include "Engine/Window/Window.h"
#include "Engine/Scene/Scene.h"
#include "entt/entt.hpp"

namespace Engine
{
	class RenderSystem {
	public:
		RenderSystem(std::shared_ptr<Shader>& shader)
			: _Shader(shader), _Registry(&Scene::Get().GetRegistry())
		{
			_Window = std::make_shared<Window>(Application::Get().GetWindow());
			float aspectRatio = (float)_Window->GetWidth() / (float)_Window->GetHeight();
			auto view = _Registry->view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				camera.SetAspectRatio(aspectRatio);
			}
			_ShadowShader = std::make_shared<Shader>("../Engine/Shaders/shadow_vertex.glsl", "../Engine/Shaders/shadow_fragment.glsl");
			_FrameBufferShader = std::make_shared<Shader>("../Engine/Shaders/framebuffer_screen_vertex.glsl", "../Engine/Shaders/framebuffer_screen_fragment.glsl");
		}

		void Draw(CameraComponent camera);
		void RenderShadowPass(const glm::mat4& lightSpaceMatrix);
		void RenderMainScene();

		void UpdateCameraSystem()
		{
			auto view = _Registry->view<CameraComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);

				_Shader->SetVec3("u_ViewPos", transform.Position);
				_Shader->SetVec3("u_LightPos", transform.Position);

				camera.UpdateCameraVectors();
			}
		}
	private:
		std::shared_ptr<Shader> _Shader;
		std::shared_ptr<Shader> _ShadowShader;
		std::shared_ptr<Shader> _FrameBufferShader;
		std::shared_ptr<Window> _Window;
		entt::registry* _Registry;
		float distance = 60.0f;
	};
}