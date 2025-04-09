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

	entt::entity Scene::CreateCamera(entt::entity& entity, const std::string& name)
	{
		entity = _Registry.create();
		_Registry.emplace<TransformComponent>(entity);
		_Registry.emplace<NameComponent>(entity, name);
		_Registry.emplace<CameraComponent>(entity);
		_Registry.emplace<MeshComponent>(entity, "Assets/Models/Camera/camera.obj");
		auto& mesh = _Registry.get<MeshComponent>(entity);
		mesh.SetVisiblity(false);
		return entity;
	}

}
