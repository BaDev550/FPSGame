#include "epch.h"
#include "Scene.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Engine/ActorClasses/IPawn.h"
#include "Engine/Core/Core.h"
#include "Engine/Scene/ECS/Entity.h"

namespace Engine
{
	Scene* Scene::instance = nullptr;
	Scene::Scene()
	{
		instance = this;
		ENGINE_ASSERT(instance, "Scene::Get is null");
		std::cout << "Scene initialized!" << std::endl;
	}

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
		_Registry.emplace<CameraComponent>(handle, &_Registry.get<TransformComponent>(handle));
		return Entity(handle);
	}

	Entity Scene::FindEntityByID(uint32_t id)
	{
		entt::entity handle = static_cast<entt::entity>(id);
		if (_Registry.valid(handle)) {
			return Entity(handle);
		}

		return Entity();
	}

	std::optional<Entity> Scene::TryFindEntityByID(uint32_t id)
	{
		entt::entity handle = static_cast<entt::entity>(id);
		if (_Registry.valid(handle)) {
			return Entity(handle);
		}
		return std::nullopt;
	}

	void Scene::Start()
	{
		auto view = _Registry.view<ActorComponent>();
		for (auto& entity : view) {
			auto& actor = _Registry.get<ActorComponent>(entity);
			actor.GetActor()->OnStart();
		}
	}

	void Scene::Update()
	{
		auto view = _Registry.view<ActorComponent>();
		for (auto& entity : view) {
			auto& actor = _Registry.get<ActorComponent>(entity);
			actor.GetActor()->OnUpdate();
		}
	}

	bool Scene::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		auto view = _Registry.view<PawnComponent>();
		for (auto& entity : view) {
			auto& pawn = _Registry.get<PawnComponent>(entity);
			pawn.GetPawn()->OnMouseMoved(e);
		}
		return true;
	}

	bool Scene::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		auto view = _Registry.view<PawnComponent>();
		for (auto& entity : view) {
			auto& pawn = _Registry.get<PawnComponent>(entity);
			pawn.GetPawn()->OnKeyPressed(e);
		}
		return true;
	}

	bool Scene::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		auto view = _Registry.view<PawnComponent>();
		for (auto& entity : view) {
			auto& pawn = _Registry.get<PawnComponent>(entity);
			pawn.GetPawn()->OnKeyReleased(e);
		}
		return true;
	}

	bool Scene::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto view = _Registry.view<PawnComponent>();
		for (auto& entity : view) {
			auto& pawn = _Registry.get<PawnComponent>(entity);
			pawn.GetPawn()->OnMouseButtonPressed(e);
		}
		return true;
	}

}
