#include "epch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Engine
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::ClearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
	}
	void OpenGLRendererAPI::DrawVertex(const std::shared_ptr<VertexArrayBuffer>& vertexArray)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());
	}
	void OpenGLRendererAPI::DrawIndexedInstanced(const std::shared_ptr<VertexArrayBuffer>& vertexArray, uint32_t instancedCount)
	{
		glDrawElementsInstanced(
			GL_TRIANGLES,
			vertexArray->GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT,
			nullptr,
			instancedCount
		);
	}
	void OpenGLRendererAPI::SetPolyMode(const int face, const int mode)
	{
		glPolygonMode(face, mode);
	}
	void OpenGLRendererAPI::SetViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}