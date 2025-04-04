#pragma once

#include "Engine/Window/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h"
#include "Engine/Window/LayerStack.h"
#include "Engine/Window/ImGui/ImGuiLayer.h"

namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
	
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_Window; }
	
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		void Render();
	
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPress(KeyPressedEvent& e);
	private:
		std::unique_ptr<Window> _Window;
	
		static Application* s_Instance;
		bool _bRunning;
	private:
		ImGuiLayer* _ImGuiLayer;
		LayerStack _LayerStack;
	};

	Application* CreateApplication();
}
