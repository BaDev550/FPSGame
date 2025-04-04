#pragma once
#include <entt/entt.hpp>

class System {
public:
	virtual void Update(entt::registry& registry, float deltaTime) = 0;
	virtual ~System() = default;
};