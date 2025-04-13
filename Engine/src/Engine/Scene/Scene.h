#pragma once
#include <entt/entt.hpp>

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Application/Application.h"
#include <optional>

namespace Engine {
	class Entity;
	class Scene
	{
	public:
		Scene();
		static Scene& Get() {
			return *instance;
		}

		entt::registry& GetRegistry() {
			ENGINE_ASSERT(instance, "Scene::GetRegistry instance is null");
			return _Registry; 
		}

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateCamera(const std::string& name = "Entity");

		void ProcessEvent(Event& event) {
			Engine::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseMovedEvent));
			dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(Scene::OnKeyPressedEvent));
			dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNCTION(Scene::OnKeyReleasedEvent));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseButtonPressed));
		}

		Entity FindEntityByID(uint32_t id);
		std::optional<Entity> TryFindEntityByID(uint32_t id);

		void Start();
		void Update();

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void SetPath(const std::string path) { this->path = path; }
		std::string& GetPath() { return path; }
	private:
		std::string path;
		entt::registry _Registry;
		static Scene* instance;
	};
}