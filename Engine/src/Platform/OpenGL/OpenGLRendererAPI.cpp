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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
	}
	void OpenGLRendererAPI::SetPolyMode(const int face, const int mode)
	{
		glPolygonMode(face, mode);
	}
}