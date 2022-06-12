#pragma once

#include <PxSimulationEventCallback.h>

namespace smile::physics
{
    class ContactListener final : public physx::PxSimulationEventCallback
    {
      public:
        virtual void onConstraintBreak( physx::PxConstraintInfo *constraints, physx::PxU32 count ) override;
        virtual void onWake( physx::PxActor **actors, physx::PxU32 count ) override;
        virtual void onSleep( physx::PxActor **actors, physx::PxU32 count ) override;
        virtual void onContact( const physx::PxContactPairHeader &pair_header,
            const physx::PxContactPair *pairs,
            physx::PxU32 pair_count ) override;
        virtual void onTrigger( physx::PxTriggerPair *pairs, physx::PxU32 count ) override;
        virtual void onAdvance( const physx::PxRigidBody *const *body_buffer,
            const physx::PxTransform *pose_buffer,
            const physx::PxU32 count ) override;
    };
}