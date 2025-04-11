#pragma once  
#include <entt/entt.hpp>  

#include "Engine/Scene/Scene.h"

namespace Engine {
    enum class EEntityType {
        Actor,
        Pawn,
        Character
    };
	class Entity {
	public:
        Entity() : _EntityType(EEntityType::Actor), _Handle(entt::null), _Registry(&Scene::Get().GetRegistry()) {}
		Entity(entt::entity handle)
			: _Handle(handle), _Registry(&Scene::Get().GetRegistry()) {
		}
        virtual ~Entity() = default;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return _Registry->emplace<T>(_Handle, std::forward<Args>(args)...);
        }

        template <typename T>
        void RemoveComponent() {
            _Registry->remove<T>(_Handle);
        }

        template <typename T>
        bool HasComponent() const {
            return _Registry->any_of<T>(_Handle);
        }

        template <typename T>
        T& GetComponent() {
            return _Registry->get<T>(_Handle);
        }

        template <typename T>
        const T& GetComponent() const {
            return _Registry->get<T>(_Handle);
        }

        void SetParent(Entity* parent) {
            _Parent = parent;
        }

        bool IsValid() {
            return _Registry->valid(_Handle);
        }

        void DestroyEntity() {
            _Registry->destroy(_Handle);
        }

        Entity* GetParent() const { return _Parent; }

        operator bool() const { return _Handle != entt::null; }
        operator entt::entity() const { return _Handle; }
        operator uint32_t() const { return (uint32_t)_Handle; }

        bool operator==(const Entity& other) const {
            return _Handle == other._Handle && &_Registry == &other._Registry;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

	private:
		entt::registry* _Registry = NULL;
        entt::entity _Handle{ entt::null };
        Entity* _Parent = NULL;
        EEntityType _EntityType;
	};
}