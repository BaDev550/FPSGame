#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void ClearColor() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& vertexArray) override;
		virtual void DrawVertex(const std::shared_ptr<VertexArrayBuffer>& vertexArray) override;
		virtual void DrawIndexedInstanced(const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instancedCount) override;
		virtual void SetPolyMode(const int face, const int mode) override;
		virtual void SetViewport(int width, int height) override;
	};
}