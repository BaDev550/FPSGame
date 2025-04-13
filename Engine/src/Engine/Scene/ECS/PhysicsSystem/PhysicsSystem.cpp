#include "epch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ECS/Components.h"
#include <thread>

namespace Engine {
	static CollisionManager g_CollisionManager;
	constexpr int cMaxPhysicsJobs = 64;
	constexpr int cMaxPhysicsBarriers = 4;
	constexpr int cCollisionSteps = 1;
	constexpr int cIntegrationSubSteps = 1;
	const uint32_t threadCount = std::thread::hardware_concurrency();

	class MyContactListener : public JPH::ContactListener {
	public:
		void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold&, JPH::ContactSettings&) override {
			g_CollisionManager.AddCollision((uint32_t)body1.GetUserData(), (uint32_t)body2.GetUserData());
			std::cout << "Collision Detected" << std::endl;
		}
	};

	PhysicsSystem* PhysicsSystem::s_Instance = nullptr;
	PhysicsSystem::PhysicsSystem()
	{
		s_Instance = this;

		JPH::RegisterDefaultAllocator();
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		_TempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
		_JobSystem = std::make_unique<JPH::JobSystemThreadPool>(cMaxPhysicsJobs, cMaxPhysicsBarriers, threadCount);

		_BroadPhaseLayerInterface = std::make_unique<MyBroadPhaseLayerInterface>();

		_PhysicsSystem.Init(1024, 0, 1024, 1024,
			*_BroadPhaseLayerInterface,
			_broadPhaseFilter,
			_PairFilter
		);

		_PhysicsSystem.SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));
		_PhysicsSystem.SetContactListener(new MyContactListener());

		_BodyInterface = &_PhysicsSystem.GetBodyInterface();
	}

	PhysicsSystem::~PhysicsSystem()
	{
		_BodyInterface = nullptr;
		_PhysicsSystem.OptimizeBroadPhase();

		_BroadPhaseLayerInterface.reset();

		JPH::UnregisterTypes();
		delete JPH::Factory::sInstance;
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		auto& registry = Scene::Get().GetRegistry();

		auto rbView = registry.view<RigidBodyComponent>();
		for (auto entity : rbView) {
			auto& rb = rbView.get<RigidBodyComponent>(entity);

			if (glm::length(rb.PendingForce) > 0.01f || glm::length(rb.ConstantForce) > 0.01f) {
				glm::vec3 totalForce = rb.PendingForce + rb.ConstantForce;
				_BodyInterface->AddForce(rb.BodyID,
					JPH::Vec3(totalForce.x, totalForce.y, totalForce.z));
				rb.PendingForce = { 0,0,0 };
			}

			JPH::Vec3 currentVel = _BodyInterface->GetLinearVelocity(rb.BodyID);
			rb.Velocity = { currentVel.GetX(), currentVel.GetY(), currentVel.GetZ() };
		}

		_PhysicsSystem.Update(deltaTime, cCollisionSteps, _TempAllocator.get(), _JobSystem.get());

		auto view = registry.view<TransformComponent, RigidBodyComponent, BoxColliderComponent>();
		for (auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			auto& rigidbody = view.get<RigidBodyComponent>(entity);

			auto bodyID = rigidbody.BodyID;
			JPH::RVec3 pos = _BodyInterface->GetPosition(bodyID);
			transform.Position = glm::vec3((float)pos.GetX(), (float)pos.GetY(), (float)pos.GetZ());
		}

		for (const auto& [id1, id2] : g_CollisionManager.GetCollidingEntities())
		{
			auto entity1 = (entt::entity)id1;
			auto entity2 = (entt::entity)id2;

			if (registry.valid(entity1) && registry.valid(entity2)) {
				auto& col1 = registry.get<BoxColliderComponent>(entity1);
				auto& col2 = registry.get<BoxColliderComponent>(entity2);

				if (col1.Mode == ECollisionMode::Enabled && col2.Mode == ECollisionMode::Enabled) {
					auto& rb1 = registry.get<RigidBodyComponent>(entity1);
					auto& rb2 = registry.get<RigidBodyComponent>(entity2);

					if (!col1.IsDynamic && !col2.IsDynamic) {
						_BodyInterface->SetLinearVelocity(rb1.BodyID, JPH::Vec3::sZero());
						_BodyInterface->SetLinearVelocity(rb2.BodyID, JPH::Vec3::sZero());
					}
				}
			}
		}
		g_CollisionManager.Clear();
	}

	void PhysicsSystem::AddBoxBody(glm::vec3 pos, glm::vec3 size, bool isDynamic, uint32_t entityID)
	{
		JPH::Vec3 halfExtents(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);

		JPH::BodyCreationSettings settings(
			new JPH::BoxShape(JPH::Vec3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)),
			JPH::RVec3(pos.x, pos.y, pos.z),
			JPH::Quat::sIdentity(),
			isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static,
			isDynamic ? Layers::MOVING : Layers::NON_MOVING
		);

		settings.mUserData = static_cast<JPH::uint64>(entityID);

		if (isDynamic) {
			settings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
			settings.mMassPropertiesOverride.mMass = 1.0f;
		}

		JPH::BodyID id = _BodyInterface->CreateAndAddBody(settings, JPH::EActivation::Activate);

		auto& registry = Scene::Get().GetRegistry();
		auto entity = static_cast<entt::entity>(entityID);
		if (registry.valid(entity)) {
			if (auto* rb = registry.try_get<RigidBodyComponent>(entity)) {
				rb->BodyID = id;
			}
		}
	}
}