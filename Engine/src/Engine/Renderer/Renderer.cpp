#include "epch.h"
#include "Renderer.h"
#include "Engine/Core/Application/Application.h"

namespace Engine
{
	Renderer::SSCeneData* Renderer::_SceneData = new Renderer::SSCeneData;
	std::shared_ptr<FrameBuffer> Renderer::_FrameBuffer;
	std::shared_ptr<Texture2D> Renderer::_ShadowMap;

	void Renderer::Init(std::unique_ptr<Window>& window)
	{
		FrameBufferSpecification spec;
		spec.Width = window->GetWidth();
		spec.Height = window->GetHeight();
		_FrameBuffer.reset(FrameBuffer::Create(spec));

		_ShadowMap = Texture2D::Create(1024, 1024);
	}

	void Renderer::BeginScene(CameraComponent& camera)
	{
		_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
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

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	};

	void Renderer::RenderShadowMap(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray, const glm::mat4& lightSpaceMatrix)
	{
		_ShadowMap->Bind();
		RenderCommand::SetViewport(_ShadowMap->GetWidth(), _ShadowMap->GetHeight());
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		RenderCommand::Clear();

		shader->Bind();
		shader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);

		_ShadowMap->Unbind();
		RenderCommand::SetViewport(_FrameBuffer->GetWidth(), _FrameBuffer->GetHeight());
	}
}