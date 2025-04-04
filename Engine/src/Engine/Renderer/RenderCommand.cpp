#include "epch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

Engine::RendererAPI* Engine::RenderCommand::s_RendererAPI = new OpenGLRendererAPI;