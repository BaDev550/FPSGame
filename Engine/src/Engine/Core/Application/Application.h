#pragma once

#include "Engine/Window/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h"
#include "Engine/Window/LayerStack.h"
#include "Engine/Window/ImGui/ImGuiLayer.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Material.h"

#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>

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

		void QueueLayer(Layer* layer);
		void QueueOverlay(Layer* overlay);

		void QueueRemoveLayer(Layer* layer);
		void QueueRemoveOverlay(Layer* overlay);

		void Close();
	private:
		void Render();
	
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> _Window;
	
		static Application* s_Instance;
		bool _bRunning;
	private:
		ImGuiLayer* _ImGuiLayer;
		LayerStack _LayerStack;

		std::vector<Layer*> _PendingLayers;
		std::vector<Layer*> _PendingOverlays;

		std::vector<Layer*> _PendingRemoveLayers;
		std::vector<Layer*> _PendingRemoveOverlays;
	};

	Application* CreateApplication();
}
