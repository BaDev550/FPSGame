#pragma once
#include <entt/entt.hpp>

typedef entt::entity EntityID;
class ECSManager {
public:
	ECSManager() = default;

	static ECSManager& Get() {
		static ECSManager instance;
		return instance;
	}

	inline static entt::registry& GetRegistry() { return _Registry; }
private:
	static entt::registry _Registry;
};
