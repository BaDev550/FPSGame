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
        Entity() : _EntityType(EEntityType::Actor), _Handle(Scene::Get().GetRegistry().create()), _Registry(&Scene::Get().GetRegistry()) {}
		Entity(entt::entity handle)
			: _Handle(handle), _Registry(&Scene::Get().GetRegistry()), _EntityType(EEntityType::Actor) {
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

        void SetParent(Entity* parent) {
			if (_Parent == parent) return;

			if (_Parent) {
				_Parent->_Childs.erase(
					std::remove(_Parent->_Childs.begin(), _Parent->_Childs.end(), this),
					_Parent->_Childs.end()
				);
			}

			_Parent = parent;
        }

		bool HasParent() {
			return _Parent != NULL;
		}

        bool IsValid() {
            return _Registry->valid(_Handle);
        }

        void DestroyEntity() {
            _Registry->destroy(_Handle);
        }

		const std::vector<Entity*>& GetChildren() const {
			return _Childs;
		}

		void AddChild(Entity* child) {
			if (child && std::find(_Childs.begin(), _Childs.end(), child) == _Childs.end()) {
                _Childs.push_back(child);
                child->SetParent(this);
			}
		}

		void RemoveChild(Entity* child) {
			auto it = std::remove(_Childs.begin(), _Childs.end(), child);
			if (it != _Childs.end()) {
                _Childs.erase(it);
                child->SetParent(nullptr);
			}
		}

		uint32_t GetEntityID() const {
			return static_cast<uint32_t>(_Handle);
		}

#ifdef E_DEBUG
        void PrintComponents() {
			std::cout << "Entity [" << (uint32_t)_Handle << "] Components:\n";

			auto& reg = *_Registry;

			if (reg.all_of<TransformComponent>(_Handle)) {
				std::cout << "- TransformComponent\n";
			}
			if (reg.all_of<NameComponent>(_Handle)) {
				std::cout << "- NameComponent: " << reg.get<NameComponent>(_Handle).name << "\n";
			}
			if (reg.all_of<MeshComponent>(_Handle)) {
				std::cout << "- SpriteRendererComponent\n";
			}
			if (reg.all_of<CameraComponent>(_Handle)) {
				std::cout << "- CameraComponent\n";
			}
			if (reg.all_of<PawnComponent>(_Handle)) {
				std::cout << "- PawnComponent\n";
			}
			if (reg.all_of<ActorComponent>(_Handle)) {
				std::cout << "- ActorComponent\n";
			}
        }
#else
        void PrintComponents() {}
#endif

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
        std::vector<Entity*> _Childs;
        EEntityType _EntityType;
	};
}