#pragma once

#include <glm/glm.hpp>
#include "VertexArrayBuffer.h"
#include <memory>

namespace Engine
{
	class RendererAPI
	{
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void ClearColor() = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& vertexArray) = 0;
		virtual void DrawVertex(const std::shared_ptr<VertexArrayBuffer>& vertexArray) = 0;
		virtual void DrawIndexedInstanced(const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instancedCount) = 0;
		virtual void SetPolyMode(const int face, const int mode) = 0;
		virtual void SetViewport(int width, int height) = 0;
		static inline API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}