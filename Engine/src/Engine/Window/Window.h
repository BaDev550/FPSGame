#pragma once
#include "GLFW/glfw3.h"
#include "Engine/Renderer/GraphicsContext.h"
#include "Engine/Events/Event.h"

namespace Engine
{
	using EventCallbackFuntion = std::function<void(Event&)>;
	struct SWindowPropeties
	{
		unsigned int Width;
		unsigned int Height;
		const char* Title;
		EventCallbackFuntion EventCallbackFn;
	
		SWindowPropeties(unsigned int width = 1280, unsigned int height = 720, const char* title = "Window")
			: Width(width), Height(height), Title(title) {
		}
	};
	class Window
	{
	public:
		Window(SWindowPropeties props);
		~Window();
	
		void Init();
		void Shutdown();
		void SwapBuffers();
		void SetVSync(bool enabled);
		void SetInputMode(int mode, int value);
		bool IsOpen();
	
		inline void SetEventCallback(const EventCallbackFuntion& callback) { _WindowProperties.EventCallbackFn = callback; }
		inline unsigned int GetWidth() const { return _WindowProperties.Width; }
		inline unsigned int GetHeight() const { return _WindowProperties.Height; }
		inline virtual void* GetNativeWindow() const { return _Window; }
	private:
		void Create();
	
		GraphicsContext* _Context;
		GLFWwindow* _Window;
		SWindowPropeties _WindowProperties;
	};
}

