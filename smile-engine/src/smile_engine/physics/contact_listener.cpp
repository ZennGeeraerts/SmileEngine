/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "contact_listener.h"

#include "smile_engine/scene/entity.h"

#include <PhysX/PxRigidActor.h>

namespace smile::physics
{
    void ContactListener::onConstraintBreak( physx::PxConstraintInfo *pConstraints, physx::PxU32 count )
    {
        PX_UNUSED( pConstraints );
        PX_UNUSED( count );
    }

    void ContactListener::onWake( physx::PxActor **ppActors, physx::PxU32 count )
    {
        for ( Uint32 i{}; i < count; ++i )
        {
            physx::PxActor &actor = *ppActors[i];
            scene::Entity &entity = *reinterpret_cast< scene::Entity * >( actor.userData );
            SM_LOG_INFO( "Physics actor waking up: UUID: %llu, Name: %s", entity.GetUUID(), entity.GetName() );
        }
    }

    void ContactListener::onSleep( physx::PxActor **ppActors, physx::PxU32 count )
    {
        for ( Uint32 i{}; i < count; ++i )
        {
            physx::PxActor &actor = *ppActors[i];
            scene::Entity &entity = *reinterpret_cast< scene::Entity * >( actor.userData );
            SM_LOG_INFO( "Physics actor going to sleep: UUID: %llu, Name: %s", entity.GetUUID(), entity.GetName() );
        }
    }

    void ContactListener::onContact( const physx::PxContactPairHeader &pairHeader,
        const physx::PxContactPair *pairs,
        physx::PxU32 pair_count )
    {
        scene::Entity &entity0 = *static_cast< scene::Entity * >( pairHeader.actors[0]->userData );
        scene::Entity &entity1 = *static_cast< scene::Entity * >( pairHeader.actors[1]->userData );

        if ( pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH )
        {
            // On collision begin
        }
        else if ( pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH )
        {
            // On collision end
        }
    }

    void ContactListener::onTrigger( physx::PxTriggerPair *pPairs, physx::PxU32 count )
    {
        scene::Entity triggerEntity = *static_cast< scene::Entity * >( pPairs->triggerActor->userData );
        scene::Entity otherEntity = *static_cast< scene::Entity * >( pPairs->otherActor->userData );

        if ( pPairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND )
        {
            // On trigger begin
        }
        else if ( pPairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST )
        {
            // On trigger end
        }
    }

    void ContactListener::onAdvance( const physx::PxRigidBody *const *ppBodyBuffer,
        const physx::PxTransform *poseBuffer,
        const physx::PxU32 count )
    {
        PX_UNUSED( ppBodyBuffer );
        PX_UNUSED( poseBuffer );
        PX_UNUSED( count );
    }
}