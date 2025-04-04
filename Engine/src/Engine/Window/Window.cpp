#include "epch.h"
#include "Window.h"
#include "Engine/Core/Core.h"
#include <iostream>
#include "Platform/OpenGL/OpenGLContext.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	static bool s_GLFWInitialized = false;
	Window::Window(SWindowPropeties props)
	{
		_WindowProperties = props;
		Init();
		Create();
	}
	
	Window::~Window()
	{
		Shutdown();
	}
	
	void Window::Init()
	{
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			ENGINE_ASSERT(success, "Failed to initialize GLFW");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			s_GLFWInitialized = true;
		}
	}
	
	void Window::Shutdown()
	{
		glfwDestroyWindow(_Window);
		glfwTerminate();
	}
	
	void Window::SwapBuffers()
	{
		glfwPollEvents();
		_Context->SwapBuffers();
	}
	
	void Window::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
	}
	
	void Window::SetInputMode(int mode, int value)
	{
		glfwSetInputMode(_Window, mode, value);
	}
	
	bool Window::IsOpen()
	{
		return !glfwWindowShouldClose(_Window);
	}
	
	void Window::Create()
	{
		_Window = glfwCreateWindow(_WindowProperties.Width, _WindowProperties.Height, _WindowProperties.Title, NULL, NULL);
		ENGINE_ASSERT(_Window != NULL, "Failed to create GLFW window");
		glfwSetWindowUserPointer(_Window, &_WindowProperties);
		SetVSync(true);
	
		_Context = new OpenGLContext(_Window);
		_Context->Init();
	
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow* window, int width, int height) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
	
			WindowResizeEvent event(width, height);
			data.EventCallbackFn(event);
			});
	
		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallbackFn(event);
			});
	
		glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS: {
				KeyPressedEvent event(key, 0);
				data.EventCallbackFn(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				data.EventCallbackFn(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(key, 1);
				data.EventCallbackFn(event);
				break;
			}
			default:
				std::cout << "Incorrect Key action!" << std::endl;
				break;
			}
			});
	
		glfwSetCharCallback(_Window, [](GLFWwindow* window, unsigned int keycode) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.EventCallbackFn(event);
			});
	
		glfwSetMouseButtonCallback(_Window, [](GLFWwindow* window, int button, int action, int mods) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.EventCallbackFn(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				data.EventCallbackFn(event);
				break;
			}
			default:
				std::cout << "Incorrect Mouse action!" << std::endl;
				break;
			}
			});
	
		glfwSetScrollCallback(_Window, [](GLFWwindow* window, double x, double y) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)x, (float)y);
			data.EventCallbackFn(event);
			});
	
		glfwSetCursorPosCallback(_Window, [](GLFWwindow* window, double xPos, double yPos) {
			SWindowPropeties& data = *(SWindowPropeties*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallbackFn(event);
			});
	}
}
