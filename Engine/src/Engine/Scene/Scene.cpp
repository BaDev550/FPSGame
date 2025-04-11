#include "epch.h"
#include "Scene.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Engine/ActorClasses/IPawn.h"

namespace Engine
{
	Entity Scene::CreateEntity(const std::string& name)
	{
		entt::entity handle = _Registry.create();
	    _Registry.emplace<TransformComponent>(handle);
	    _Registry.emplace<NameComponent>(handle, name);
	    return Entity(handle);
	}

	Entity Scene::CreateCamera(const std::string& name)
	{
		entt::entity handle = _Registry.create();
		_Registry.emplace<TransformComponent>(handle);
		_Registry.emplace<NameComponent>(handle, name);
		_Registry.emplace<CameraComponent>(handle);
		return Entity(handle);
	}

	bool Scene::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		for (auto& entity : _Registry.view<IPawn>()) {
			IPawn& pawn = _Registry.get<IPawn>(entity);
			pawn.OnMouseMoved(e);
		}
		return true;
	}

	bool Scene::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		for (auto& entity : _Registry.view<IPawn>()) {
			IPawn& pawn = _Registry.get<IPawn>(entity);
			pawn.OnKeyPressed(e);
		}
		return true;
	}
}
