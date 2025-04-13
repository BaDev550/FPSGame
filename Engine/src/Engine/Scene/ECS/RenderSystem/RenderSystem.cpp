#include "epch.h"
#include "RenderSystem.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine {
	void RenderSystem::Draw(CameraComponent camera)
	{
		UpdateCameraSystem();

		glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		glm::mat4 lightView = glm::lookAt(-lightDir * distance, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		_Shader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

		Renderer::BeginScene(camera);

		RenderShadowPass(lightSpaceMatrix);
		RenderMainScene();
		//_DebugRenderer.RenderWireframeBoxes(_DebugShader);

		Renderer::EndScene();

		Renderer::RenderFrameBufferScreen(_FrameBufferShader);
	}

	void RenderSystem::RenderShadowPass(const glm::mat4& lightSpaceMatrix)
	{
		_ShadowShader->Bind();

		for (auto entity : _Registry->view<TransformComponent, MeshComponent>()) {
			auto& transform = _Registry->get<TransformComponent>(entity);
			auto& mesh = _Registry->get<MeshComponent>(entity);
			if (!mesh.bDrawShadows)
				continue;

			if (mesh.ModelLoaded()) {
				mesh.DrawShadow(_ShadowShader, transform, lightSpaceMatrix);
			}
		}
	}

	void RenderSystem::RenderMainScene()
	{
		_Shader->Bind();

		for (auto entity : _Registry->view<TransformComponent, MeshComponent>()) {
			auto& transform = _Registry->get<TransformComponent>(entity);
			auto& mesh = _Registry->get<MeshComponent>(entity);
			if (!mesh.bVisible)
				continue;

			if (mesh.ModelLoaded()) {
				mesh.Draw(_Shader, transform);
			}
		}
	}

}