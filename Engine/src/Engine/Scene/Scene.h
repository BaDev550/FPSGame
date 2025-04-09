#pragma once
#include "entt/entt.hpp"

namespace Engine {
	class Scene
	{
	public:
		Scene() {}
		inline entt::registry& GetRegistry() { return _Registry; }

		entt::entity CreateEntity(entt::entity& entity, const std::string& name = "Entity");
		entt::entity CreateCamera(entt::entity& entity, const std::string& name = "Entity");

		void SetPath(const std::string path) { this->path = path; }
		inline std::string& GetPath() { return path; }
	private:
		std::string path;
		entt::registry _Registry;
	};
}