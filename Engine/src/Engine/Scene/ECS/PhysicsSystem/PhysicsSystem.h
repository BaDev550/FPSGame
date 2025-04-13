#pragma once
#include "Jolt/Jolt.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Jolt/RegisterTypes.h"
#include "Engine/Events/Event.h"
#include <glm/glm.hpp>

namespace std {
	template <>
	struct hash<std::pair<uint32_t, uint32_t>> {
		size_t operator()(const std::pair<uint32_t, uint32_t>& p) const {
			size_t h1 = hash<uint32_t>()(p.first);
			size_t h2 = hash<uint32_t>()(p.second);
			return h1 ^ (h2 << 1);
		}
	};
}

namespace Engine {
	using EventCallbackFuntion = std::function<void(Event&)>;
	class CollisionManager {
	public:
		void AddCollision(uint32_t id1, uint32_t id2) {
			m_CollidingEntities.insert({ id1, id2 });
		}

		void RemoveCollision(uint32_t id1, uint32_t id2) {
			m_CollidingEntities.insert({ id1, id2 });
		}

		void Clear() {
			m_CollidingEntities.clear();
		}

		const std::unordered_set<std::pair<uint32_t, uint32_t>>& GetCollidingEntities() const {
			return m_CollidingEntities;
		}

	private:
		std::unordered_set<std::pair<uint32_t, uint32_t>> m_CollidingEntities;
	};

	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	class MyBroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
	{
	public:
		MyBroadPhaseLayerInterface()
		{
			mObjectToBroadPhase[Layers::NON_MOVING] = JPH::BroadPhaseLayer(0);
			mObjectToBroadPhase[Layers::MOVING] = JPH::BroadPhaseLayer(1);
		}

		virtual unsigned int GetNumBroadPhaseLayers() const override { return 2; }

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
			return mObjectToBroadPhase[inLayer];
		}

	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer layer1, JPH::ObjectLayer layer2) const override
		{
			return true;
		}
	};

	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer) const override
		{
			return true;
		}
	};

	class PhysicsSystem {
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(float deltaTime);
		void AddBoxBody(glm::vec3 pos, glm::vec3 size, bool isDynamic, uint32_t entityID);
		JPH::BodyInterface& GetBodyInterface() { return *_BodyInterface; }

		static PhysicsSystem& Get() {
			return *s_Instance;
		}
	private:
		std::unique_ptr<JPH::TempAllocatorImpl> _TempAllocator = nullptr;
		std::unique_ptr<JPH::JobSystemThreadPool> _JobSystem = nullptr;
		std::unique_ptr<MyBroadPhaseLayerInterface> _BroadPhaseLayerInterface;
		ObjectVsBroadPhaseLayerFilterImpl _broadPhaseFilter;
		ObjectLayerPairFilterImpl _PairFilter;
		JPH::PhysicsSystem _PhysicsSystem;

		JPH::BodyInterface* _BodyInterface = nullptr;
		static PhysicsSystem* s_Instance;
		EventCallbackFuntion _EventCallbackFn;
	};
}