#include "smpch.h"
#include "contact_listener.h"

#include "smile_engine/scene/entity.h"

#include <PxRigidActor.h>

namespace smile::physics
{
    void ContactListener::onConstraintBreak( physx::PxConstraintInfo *constraints, physx::PxU32 count )
    {
        PX_UNUSED( constraints );
        PX_UNUSED( count );
    }

    void ContactListener::onWake( physx::PxActor **actors, physx::PxU32 count )
    {
        for ( Uint32 i{}; i < count; ++i )
        {
            physx::PxActor &actor = *actors[i];
            scene::Entity &entity = *reinterpret_cast< scene::Entity * >( actor.userData );
            SM_LOG_INFO( "Physics actor waking up: UUID: %llu, Name: %s", entity.getUUID(), entity.getName() );
        }
    }

    void ContactListener::onSleep( physx::PxActor **actors, physx::PxU32 count )
    {
        for ( Uint32 i{}; i < count; ++i )
        {
            physx::PxActor &actor = *actors[i];
            scene::Entity &entity = *reinterpret_cast< scene::Entity * >( actor.userData );
            SM_LOG_INFO( "Physics actor going to sleep: UUID: %llu, Name: %s", entity.getUUID(), entity.getName() );
        }
    }

    void ContactListener::onContact( const physx::PxContactPairHeader &pair_header,
        const physx::PxContactPair *pairs,
        physx::PxU32 pair_count )
    {
        scene::Entity &entity0 = *static_cast< scene::Entity * >( pair_header.actors[0]->userData );
        scene::Entity &entity1 = *static_cast< scene::Entity * >( pair_header.actors[1]->userData );

        if ( pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH )
        {
            // On collision begin
        }
        else if ( pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH )
        {
            // On collision end
        }
    }

    void ContactListener::onTrigger( physx::PxTriggerPair *pairs, physx::PxU32 count )
    {
        scene::Entity trigger_entity = *static_cast< scene::Entity * >( pairs->triggerActor->userData );
        scene::Entity other_entity = *static_cast< scene::Entity * >( pairs->otherActor->userData );

        if ( pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND )
        {
            // On trigger begin
        }
        else if ( pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST )
        {
            // On trigger end
        }
    }

    void ContactListener::onAdvance( const physx::PxRigidBody *const *body_buffer,
        const physx::PxTransform *pose_buffer,
        const physx::PxU32 count )
    {
        PX_UNUSED( body_buffer );
        PX_UNUSED( pose_buffer );
        PX_UNUSED( count );
    }
}