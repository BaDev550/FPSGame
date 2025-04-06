#include "epch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Engine
{
	FrameBuffer* FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not suported"); return NULL;
		case RendererAPI::API::OpenGL: return new OpenGLFramebuffer(spec);
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Render only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}
}
