#pragma once

#include "Engine/Renderer/Buffers.h"
#include "Engine/Renderer/VertexArrayBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ECS/Components.h"

namespace Engine {
	class DebugRenderer
	{
	public:
		DebugRenderer() {

		}
		~DebugRenderer() {

		}

		void Setup() {
			std::vector<Vertex> vertices = {
				// Front face
				{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
				// Back face
				{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}
			};

			std::vector<uint32_t> indices = {
				0,1,1,2,2,3,3,0,
				4,5,5,6,6,7,7,4,
				0,4,1,5,2,6,3,7
			};

			_vertexBuffer.reset(VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex)));
			_indexBuffer.reset(IndexBuffer::Create(indices.data(), indices.size()));

			BufferLayout layout = { { ShaderDataType::Vec3, "aPos" } };
			_vertexBuffer->SetLayout(layout);

			_VAO.reset(VertexArrayBuffer::Create());
			_VAO->AddVertexBuffer(_vertexBuffer);
			_VAO->SetIndexBuffer(_indexBuffer);
		}
		void RenderWireframeBoxes(std::shared_ptr<Shader>& shader)
		{
			auto& registry = Scene::Get().GetRegistry();
			auto view = registry.view<TransformComponent, BoxColliderComponent>();

			for (auto entity : view) {
				auto& transform = view.get<TransformComponent>(entity);
				auto& collider = view.get<BoxColliderComponent>(entity);

				glm::mat4 model = transform.GetModelMatrix();
				model = glm::scale(model, collider.Size);

				shader->Bind();
				shader->SetMat4("model", model);
			}
			Renderer::Submit(shader, _VAO);
		}

	private:
		std::shared_ptr<VertexArrayBuffer> _VAO;
		std::shared_ptr<VertexBuffer> _vertexBuffer;
		std::shared_ptr<IndexBuffer> _indexBuffer;
	};
}