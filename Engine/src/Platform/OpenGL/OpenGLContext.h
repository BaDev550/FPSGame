#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Engine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
	
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* _WindowHandle;
	};
}
