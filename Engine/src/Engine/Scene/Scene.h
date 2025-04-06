#pragma once
#include "entt/entt.hpp"

namespace Engine {
	class Scene
	{
	public:
		Scene() {}
		inline entt::registry& GetRegistry() { return _Registry; }

		entt::entity CreateEntity(entt::entity& entity, const std::string& name = "Entity");

		// to-do implement
		void NewScene() {}
		void LoadScene() {}
	private:
		entt::registry _Registry;
	};
}