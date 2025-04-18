#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class RenderCommand
	{
	public:
		inline static void SetViewport(int width, int height) {
			s_RendererAPI->SetViewport(width, height);
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear() {
			s_RendererAPI->Clear();
		}
		inline static void ClearColor() {
			s_RendererAPI->ClearColor();
		}
		inline static void DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}
		inline static void DrawIndexedInstanced(const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instancedCount) {
			s_RendererAPI->DrawIndexedInstanced(vertexArray, instancedCount);
		}
		inline static void DrawVertex(const std::shared_ptr<VertexArrayBuffer>& vertexArray) {
			s_RendererAPI->DrawVertex(vertexArray);
		}
		inline static void SetPolyMode(const int face, const int mode) {
			s_RendererAPI->SetPolyMode(face, mode);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}