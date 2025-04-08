#include "epch.h"
#include "Application.h"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{	
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		ENGINE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	
		_Window = std::make_unique<Window>(SWindowPropeties(1280, 720, "Window"));
		_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
		_bRunning = true;
	
		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);
		
		Renderer::Init(_Window);
		for (Layer* layer : _LayerStack)
			layer->SetWindow(_Window.get());
	}
	
	Application::~Application()
	{

	}
	
	void Application::Render()
	{
		for (Layer* layer : _LayerStack)
			layer->OnUpdate();
	
		_ImGuiLayer->Begin();
		for (Layer* layer : _LayerStack)
			layer->OnImGuiRender();
		_ImGuiLayer->End();
	}
	
	void Application::PushLayer(Layer* layer)
	{
		_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	
	void Application::PushOverlay(Layer* overlay)
	{
		_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::QueueLayer(Layer* layer)
	{
		_PendingLayers.push_back(layer);
	}

	void Application::QueueOverlay(Layer* overlay)
	{
		_PendingOverlays.push_back(overlay);
	}

	void Application::QueueRemoveLayer(Layer* layer) {
		_PendingRemoveLayers.push_back(layer);
	}

	void Application::QueueRemoveOverlay(Layer* overlay) {
		_PendingRemoveOverlays.push_back(overlay);
	}

	void Application::Close()
	{
		_bRunning = false;
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::GetFramebuffer()->Resize(e.GetWidth(), e.GetHeight());
		return true;
	}
	
	float lastFrameTime = 0.0f;
	void Application::Run()
	{
		while (_Window->IsOpen() && _bRunning) {
			Render();

			for (Layer* layer : _PendingLayers)
				PushLayer(layer);
			_PendingLayers.clear();

			for (Layer* overlay : _PendingOverlays)
				PushOverlay(overlay);
			_PendingOverlays.clear();

			for (Layer* layer : _PendingRemoveLayers)
				_LayerStack.RemoveLayer(layer);
			_PendingRemoveLayers.clear();

			for (Layer* overlay : _PendingRemoveOverlays)
				_LayerStack.RemoveOverlay(overlay);
			_PendingRemoveOverlays.clear();

			_Window->SwapBuffers();
		}
	}
	
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResize));

		for (auto it = _LayerStack.end(); it != _LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
}
