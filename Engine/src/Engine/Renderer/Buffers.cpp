#include "epch.h"
#include "Buffers.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffers.h"
#include "Engine/Core/Core.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(Vertex* vertices, uint32_t size)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not suported"); return NULL;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Renderer engine only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not suported"); return NULL;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Renderer engine only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}
}