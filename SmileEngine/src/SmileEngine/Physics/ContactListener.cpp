#include "smpch.h"
#include "ContactListener.h"

#include "SmileEngine/Scene/Entity.h"

#include <PxRigidActor.h>

namespace Smile
{
	void ContactListener::onConstraintBreak(physx::PxConstraintInfo* pConstraints, physx::PxU32 count)
	{
		PX_UNUSED(pConstraints);
		PX_UNUSED(count);
	}

	void ContactListener::onWake(physx::PxActor** ppActors, physx::PxU32 count)
	{
		for (uint32_t i{}; i < count; ++i)
		{
			physx::PxActor& actor = *ppActors[i];
			Entity& entity = *reinterpret_cast<Entity*>(actor.userData);
			SM_LOG_INFO("Physics actor waking up: UUID: %llu, Name: %s", entity.GetUUID(), entity.GetName());
		}
	}

	void ContactListener::onSleep(physx::PxActor** ppActors, physx::PxU32 count)
	{
		for (uint32_t i{}; i < count; ++i)
		{
			physx::PxActor& actor = *ppActors[i];
			Entity& entity = *reinterpret_cast<Entity*>(actor.userData);
			SM_LOG_INFO("Physics actor going to sleep: UUID: %llu, Name: %s", entity.GetUUID(), entity.GetName());
		}
	}

	void ContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pPairs, physx::PxU32 nbPairs)
	{
		Entity& entity0 = *static_cast<Entity*>(pairHeader.actors[0]->userData);
		Entity& entity1 = *static_cast<Entity*>(pairHeader.actors[1]->userData);
	
		if (pPairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
		{
			// On collision begin
		}
		else if (pPairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
		{
			// On collision end
		}
	}

	void ContactListener::onTrigger(physx::PxTriggerPair* pPairs, physx::PxU32 count)
	{
		Entity triggerEntity = *static_cast<Entity*>(pPairs->triggerActor->userData);
		Entity otherEntity = *static_cast<Entity*>(pPairs->otherActor->userData);

		if (pPairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			// On trigger begin
		}
		else if (pPairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			// On trigger end
		}
	}

	void ContactListener::onAdvance(const physx::PxRigidBody* const* ppBodyBuffer, const physx::PxTransform* pPoseBuffer, const physx::PxU32 count)
	{
		PX_UNUSED(ppBodyBuffer);
		PX_UNUSED(pPoseBuffer);
		PX_UNUSED(count);
	}
}