#include "epch.h"
#include "Renderer.h"
#include "Engine/Core/Application/Application.h"

namespace Engine
{
	Renderer::SSCeneData* Renderer::_SceneData = new Renderer::SSCeneData;
	std::shared_ptr<FrameBuffer> Renderer::_FrameBuffer;

	void Renderer::Init(std::unique_ptr<Window>& window)
	{
		FrameBufferSpecification spec;
		spec.Width = window->GetWidth();
		spec.Height = window->GetHeight();
		_FrameBuffer.reset(FrameBuffer::Create(spec));
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
}