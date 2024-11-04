/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/physics/physics_world.h"

namespace physx
{
    class PxScene;
}

namespace smile::physics
{
    class PhysXPhysicsWorld final : public PhysicsWorld
    {
      public:
        PhysXPhysicsWorld( PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings );

        void *GetInternal() const override
        {
            return m_pScene;
        }

      private:
        physx::PxScene *m_pScene;
    };
}