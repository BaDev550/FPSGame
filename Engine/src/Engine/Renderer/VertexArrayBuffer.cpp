#include "epch.h"
#include "VertexArrayBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArrayBuffer.h"
#include "Engine/Core/Core.h"

namespace Engine
{
	VertexArrayBuffer* VertexArrayBuffer::Create()
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not suported"); return NULL;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArrayBuffer();
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Render only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}
}