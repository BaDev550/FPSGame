#pragma once
#include <entt/entt.hpp>

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Core/Core.h"

namespace Engine {
	class Entity;
	class Scene
	{
	public:
		Scene() {}
		static Scene& Get() {
			static Scene instance;
			return instance;
		}

		entt::registry& GetRegistry() { return _Registry; }

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateCamera(const std::string& name = "Entity");

		void ProcessEvent(Event& event) {
			Engine::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseMovedEvent));
			dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(Scene::OnKeyPressedEvent));
		}

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		void SetPath(const std::string path) { this->path = path; }
		std::string& GetPath() { return path; }
	private:
		std::string path;
		entt::registry _Registry;
	};
}