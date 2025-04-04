#include "epch.h"
#include "Renderer.h"
#include "Engine/Core/Application/Application.h"

namespace Engine
{
	Renderer::SSCeneData* Renderer::_SceneData = new Renderer::SSCeneData;
	void Renderer::BeginScene(Camera& camera)
	{
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", _SceneData->ViewProjectionMatrix);
	
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	};
}