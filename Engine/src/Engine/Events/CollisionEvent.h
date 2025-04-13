#pragma once

#include "Event.h"
#include "Engine/Scene/ECS/Entity.h"
#include "Engine/Scene/ECS/Components.h"

namespace Engine
{
	class CollisionEvent : public Event
	{
	public:
		Entity GetOtherActor() const { return _OtherActor; }

		EVENT_CLASS_CATEGORY(EventCategoryCollision)
	protected:
		CollisionEvent(Entity otherActor)
			: _OtherActor(otherActor) {
		}

		Entity _OtherActor;
	};

	class OnCollisionBegin : public CollisionEvent
	{
	public:
		OnCollisionBegin(Entity otherActor)
			: CollisionEvent(otherActor) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CollisionBeginEvent: " << _OtherActor.GetComponent<NameComponent>().name.c_str() << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CollisionDetected)
	};

	class OnCollisionEnd : public CollisionEvent
	{
	public:
		OnCollisionEnd(Entity otherActor)
			: CollisionEvent(otherActor) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CollisionEndEvent: " << _OtherActor.GetComponent<NameComponent>().name.c_str() << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CollisionEnded)
	};
}
