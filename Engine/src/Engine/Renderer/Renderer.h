#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArrayBuffer>& vertexArray);
	
		inline static RendererAPI::API CurrentAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SSCeneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SSCeneData* _SceneData;
	};
}