#include "epch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ECS/Components.h"
#include <thread>

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
	std::unordered_set<std::pair<uint32_t, uint32_t>> g_CollidingEntities;
	constexpr int cMaxPhysicsJobs = 64;
	constexpr int cMaxPhysicsBarriers = 4;
	constexpr int cCollisionSteps = 1;
	constexpr int cIntegrationSubSteps = 1;
	const uint32_t threadCount = std::thread::hardware_concurrency();

	class MyBroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
	{
	public:
		MyBroadPhaseLayerInterface()
		{
			mBroadPhaseLayers[0] = JPH::BroadPhaseLayer(0);
			mBroadPhaseLayers[1] = JPH::BroadPhaseLayer(1);
		}

		virtual unsigned int GetNumBroadPhaseLayers() const override { return 2; }
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override
		{
			return mBroadPhaseLayers[layer];
		}
	private:
		JPH::BroadPhaseLayer mBroadPhaseLayers[2];
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

	class MyContactListener : public JPH::ContactListener
	{
	public:
		void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2,
			const JPH::ContactManifold&, JPH::ContactSettings&) override
		{
			g_CollidingEntities.insert({ (uint32_t)body1.GetUserData(), (uint32_t)body2.GetUserData() });
		}
	};

	PhysicsSystem::PhysicsSystem()
	{
		JPH::RegisterDefaultAllocator();
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		_TempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
		_JobSystem = std::make_unique<JPH::JobSystemThreadPool>(cMaxPhysicsJobs, cMaxPhysicsBarriers, threadCount);

		MyBroadPhaseLayerInterface broadPhaseLayerInterface;
		ObjectVsBroadPhaseLayerFilterImpl broadPhaseFilter;
		ObjectLayerPairFilterImpl pairFilter;

		_PhysicsSystem.Init(1024, 0, 1024, 1024,
			broadPhaseLayerInterface,
			broadPhaseFilter,
			pairFilter
		);

		_BodyInterface = &_PhysicsSystem.GetBodyInterface();
	}

	PhysicsSystem::~PhysicsSystem()
	{
		JPH::UnregisterTypes();
		delete JPH::Factory::sInstance;
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		auto& registry = Scene::Get().GetRegistry();

		auto view = registry.view<TransformComponent, RigidBodyComponent, BoxColliderComponent>();
		for (auto entity : view) {
			auto& rigidbody = view.get<RigidBodyComponent>(entity);
			auto& collider = view.get<BoxColliderComponent>(entity);

			if (collider.IsDynamic && collider.Mode == ECollisionMode::Enabled) {
				JPH::BodyID id = rigidbody.BodyID;

				if (_BodyInterface->IsActive(id)) {
					glm::vec3 gravity(0.0f, -9.81f, 0.0f);
					_BodyInterface->AddForce(id, JPH::Vec3(gravity.x, gravity.y, gravity.z));
				}
			}
		}

		_PhysicsSystem.Update(deltaTime, cCollisionSteps, _TempAllocator.get(), _JobSystem.get());

		for (const auto& [id1, id2] : g_CollidingEntities)
		{
			auto entity1 = (entt::entity)id1;
			auto entity2 = (entt::entity)id2;

			if (registry.valid(entity1) && registry.valid(entity2)) {
				auto& col1 = registry.get<BoxColliderComponent>(entity1);
				auto& col2 = registry.get<BoxColliderComponent>(entity2);

				if (col1.Mode == ECollisionMode::Enabled && col2.Mode == ECollisionMode::Enabled) {
					auto& rb1 = registry.get<RigidBodyComponent>(entity1);
					auto& rb2 = registry.get<RigidBodyComponent>(entity2);

					_BodyInterface->SetLinearVelocity(rb1.BodyID, JPH::Vec3::sZero());
					_BodyInterface->SetLinearVelocity(rb2.BodyID, JPH::Vec3::sZero());
				}
			}
		}
		g_CollidingEntities.clear();

		for (auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			auto& rigidbody = view.get<RigidBodyComponent>(entity);

			auto bodyID = rigidbody.BodyID;
			JPH::RVec3 pos = _BodyInterface->GetPosition(bodyID);
			transform.Position = glm::vec3((float)pos.GetX(), (float)pos.GetY(), (float)pos.GetZ());
		}
	}

	void PhysicsSystem::AddBoxBody(glm::vec3 pos, glm::vec3 size, bool isDynamic, uint32_t entityID)
	{
		JPH::BodyCreationSettings settings(
			new JPH::BoxShape(JPH::Vec3(size.x, size.y, size.z)),
			JPH::RVec3(pos.x, pos.y, pos.z),
			JPH::Quat::sIdentity(),
			isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static,
			isDynamic ? 1 : 0
		);

		settings.mUserData = (uint64_t)entityID;

		JPH::BodyID id = _BodyInterface->CreateAndAddBody(settings, JPH::EActivation::Activate);
		std::cout << "Added Body with ID: " << id.GetIndexAndSequenceNumber() << "\n";
	}
}