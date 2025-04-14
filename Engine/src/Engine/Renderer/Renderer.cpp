#include "epch.h"
#include "Renderer.h"
#include "Engine/Core/Application/Application.h"

#define DRAW_INSTANCED = 0
namespace Engine
{
	Renderer::SSCeneData* Renderer::_SceneData = new Renderer::SSCeneData;
	std::shared_ptr<FrameBuffer> Renderer::_FrameBuffer;
	std::shared_ptr<FrameBuffer> Renderer::_ShadowMapFramebuffer;
	std::shared_ptr<Texture2D> Renderer::_ShadowMap;
	std::shared_ptr<Texture2D> Renderer::_ScreenTexture;

	void Renderer::Init(std::unique_ptr<Window>& window)
	{
		FrameBufferSpecification spec;
		spec.Width = window->GetWidth();
		spec.Height = window->GetHeight();
		_FrameBuffer.reset(FrameBuffer::Create(spec));

		uint32_t screenTextureID = _FrameBuffer->GetColorAttachmentFramebufferID();
		_ScreenTexture = Texture2D::CreateFromID(screenTextureID, window->GetWidth(), window->GetHeight());

		FrameBufferSpecification shadowSpec;
		shadowSpec.Width = 1024;
		shadowSpec.Height = 1024;
		shadowSpec.type = EFrameBufferType::ShadowMap;
		_ShadowMapFramebuffer.reset(FrameBuffer::Create(shadowSpec));

		uint32_t shadowDepthTexID = _ShadowMapFramebuffer->GetDepthAttachmentID();
		_ShadowMap = Texture2D::CreateFromID(shadowDepthTexID, 1024, 1024);
	}

	void Renderer::BeginScene(CameraComponent& camera)
	{
		_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
		_FrameBuffer->Unbind();
	}
	
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", _SceneData->ViewProjectionMatrix);

		_ShadowMap->Bind(1);
		shader->SetInt("u_ShadowMap", 1);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitInstanced(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instanceCount)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", _SceneData->ViewProjectionMatrix);

		_ShadowMap->Bind(1);
		shader->SetInt("u_ShadowMap", 1);

		vertexArray->Bind();
		RenderCommand::DrawIndexedInstanced(vertexArray, instanceCount);
	}

	void Renderer::RenderShadowMap(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray, const glm::mat4& lightSpaceMatrix)
	{
		_ShadowMapFramebuffer->Bind();

		RenderCommand::SetViewport(_ShadowMap->GetWidth(), _ShadowMap->GetHeight());
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();

		shader->Bind();
		shader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);

		_ShadowMapFramebuffer->Unbind();

		_FrameBuffer->Bind();
		RenderCommand::SetViewport(_FrameBuffer->GetWidth(), _FrameBuffer->GetHeight());
	}

	void Renderer::RenderFrameBufferScreen(const std::shared_ptr<Shader>& shader) {
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		RenderCommand::ClearColor();

		shader->Bind();

		_ScreenTexture->Bind(0);
		shader->SetInt("screenTexture", 0);

		_FrameBuffer->GetScreenBuffer()->Bind();
		RenderCommand::DrawVertex(_FrameBuffer->GetScreenBuffer());
	}
}