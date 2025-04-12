#pragma once

#include "Engine/Scene/ECS/Entity.h"
#include "Engine/Scene/ECS/Components.h"

namespace Engine {
	class IActor : public Entity
	{
	public:
		IActor(std::string name) {
			AddComponent<NameComponent>(name);
			AddComponent<TransformComponent>();
			AddComponent<ActorComponent>(this);
		}
		virtual ~IActor() = default;

		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
	};
}