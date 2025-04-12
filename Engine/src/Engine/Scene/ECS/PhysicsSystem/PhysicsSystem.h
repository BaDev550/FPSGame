#pragma once
#include "Jolt/Jolt.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Jolt/RegisterTypes.h"
#include <glm/glm.hpp>

namespace Engine {
	class PhysicsSystem {
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(float deltaTime);
		void AddBoxBody(glm::vec3 pos, glm::vec3 size, bool isDynamic, uint32_t entityID);
	private:
		std::unique_ptr<JPH::TempAllocatorImpl> _TempAllocator = nullptr;
		std::unique_ptr<JPH::JobSystemThreadPool> _JobSystem = nullptr;
		JPH::PhysicsSystem _PhysicsSystem;

		JPH::BodyInterface* _BodyInterface = nullptr;
	};
}