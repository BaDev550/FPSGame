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

	void Application::LoadScene(const std::string& path, Scene& scene)
	{
		auto& registry = scene.GetRegistry();
		auto view = registry.view<NameComponent, TransformComponent, MeshComponent>();
		for (auto entity : view) {
			if (registry.any_of<NameComponent>(entity) || registry.any_of<TransformComponent>(entity) || registry.any_of<MeshComponent>(entity)) {
				registry.destroy(entity);
			}
		}

		YAML::Node data = YAML::LoadFile(path);
		auto entities = data["Entities"];

		for (auto entityNode : entities) {
			entt::entity entity = scene.GetRegistry().create();

			auto tagNode = entityNode["NameComponent"];
			if (tagNode) {
				auto& tag = scene.GetRegistry().emplace<NameComponent>(entity);
				tag.name = tagNode["Name"].as<std::string>();
			}

			auto transformNode = entityNode["TransformComponent"];
			if (transformNode) {
				auto& tc = scene.GetRegistry().emplace<TransformComponent>(entity);
				tc.Position = transformNode["Position"].as<glm::vec3>();
				tc.Rotation = transformNode["Rotation"].as<glm::vec3>();
				tc.Scale = transformNode["Scale"].as<glm::vec3>();
			}

			auto modelNode = entityNode["MeshComponent"];
			if (modelNode) {
				std::string path = modelNode["ModelPath"].as<std::string>();

				scene.GetRegistry().emplace<MeshComponent>(entity, path);
			}
		}
	}

	void Application::SaveScene(const std::string& path, Scene& scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = scene.GetRegistry().view<NameComponent, TransformComponent, MeshComponent>();

		for (auto entity : view) {
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << (uint32_t)entity;

			if (scene.GetRegistry().any_of<NameComponent>(entity)) {
				auto& tag = scene.GetRegistry().get<NameComponent>(entity);
				out << YAML::Key << "NameComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Name" << YAML::Value << tag.name;
				out << YAML::EndMap;
			}

			if (scene.GetRegistry().any_of<TransformComponent>(entity)) {
				auto& tc = scene.GetRegistry().get<TransformComponent>(entity);
				out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Position" << YAML::Value << tc.Position;
				out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
				out << YAML::EndMap;
			}

			if (scene.GetRegistry().any_of<MeshComponent>(entity)) {
				auto& tc = scene.GetRegistry().get<MeshComponent>(entity);
				out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "ModelPath" << YAML::Value << tc._Model->GetPath();
				out << YAML::EndMap;
			}

			out << YAML::EndMap; // End Entity
		}

		out << YAML::EndSeq; // End Entities
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
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
