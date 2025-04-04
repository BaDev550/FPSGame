#include "epch.h"
#include "Application.h"
#include "Engine/Core/Core.h"

namespace Engine
{	
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		ENGINE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	
		_Window = std::make_unique<Window>(SWindowPropeties(1280, 720, "Engine"));
		_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
		_bRunning = true;
	
		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);

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
	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_bRunning = false;
		return true;
	}
	
	float lastFrameTime = 0.0f;
	void Application::Run()
	{
		while (_Window->IsOpen() && _bRunning) {
			Render();
			_Window->SwapBuffers();
		}
	}
	
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));

		for (auto it = _LayerStack.end(); it != _LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
}
