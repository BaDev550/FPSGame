#include "epch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <iostream>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: _WindowHandle(windowHandle)
	{
		ENGINE_ASSERT(_WindowHandle, "Window Handle is NULL");
	}
	
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_WindowHandle);
	
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(status, "Failed to init GLAD");
	
		std::cout << "|-OpenGL Info" << std::endl;
		std::cout << "|----Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl;
		std::cout << "|----Vendor: " << (char*)glGetString(GL_VENDOR) << std::endl;
		std::cout << "|----Version: " << (char*)glGetString(GL_VERSION) << std::endl;
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_WindowHandle);
	}
}