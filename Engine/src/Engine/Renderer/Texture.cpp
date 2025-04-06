#include "epch.h"
#include "Texture.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Engine
{	
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, const std::string& type)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not suported"); return NULL;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path, type);
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Render engine only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RenderAPI::None not supported"); return NULL;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height);
		default:
			ENGINE_ASSERT(false, "Incorrect Render API Vortex Render engine only supports OpenGL for now");
			return NULL;
		}
		return NULL;
	}
}
