#pragma once

#include "Engine/Scene/ECS/Entity.h"
#include "Engine/Scene/ECS/Components.h"

namespace Engine {
	class IActor : public Entity
	{
	public:
		IActor(std::string name) : Entity(Scene::Get().GetRegistry().create()) {
			AddComponent<NameComponent>(name);
			AddComponent<TransformComponent>();
		}
		virtual ~IActor() = default;
	};
}