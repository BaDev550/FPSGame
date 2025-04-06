#include "epch.h"
#include "Scene.h"
#include "ECS/Components.h"

namespace Engine
{
	entt::entity Scene::CreateEntity(entt::entity& entity, const std::string& name)
	{
	    entity = _Registry.create();
	    _Registry.emplace<TransformComponent>(entity);
	    _Registry.emplace<NameComponent>(entity, name);
	    return entity;
	}
}
