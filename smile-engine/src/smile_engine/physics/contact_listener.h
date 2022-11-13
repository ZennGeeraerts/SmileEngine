#pragma once

#include <PxSimulationEventCallback.h>

namespace smile::physics
{
    class ContactListener final : public physx::PxSimulationEventCallback
    {
      public:
        virtual void onConstraintBreak( physx::PxConstraintInfo *pConstraints, physx::PxU32 count ) override;
        virtual void onWake( physx::PxActor **ppActors, physx::PxU32 count ) override;
        virtual void onSleep( physx::PxActor **ppActors, physx::PxU32 count ) override;
        virtual void onContact( const physx::PxContactPairHeader &pairHeader,
            const physx::PxContactPair *pPairs,
            physx::PxU32 pairCount ) override;
        virtual void onTrigger( physx::PxTriggerPair *pPairs, physx::PxU32 count ) override;
        virtual void onAdvance( const physx::PxRigidBody *const *ppBodyBuffer,
            const physx::PxTransform *pPoseBuffer,
            const physx::PxU32 count ) override;
    };
}