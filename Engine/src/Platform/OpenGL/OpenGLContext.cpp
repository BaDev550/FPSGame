#include "epch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <iostream>

namespace Engine
{
	void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		std::cout << "[OpenGL Debug] " << message << std::endl;
	}

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

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_WindowHandle);
	}
}