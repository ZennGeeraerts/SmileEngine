/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/physics/physics_engine.h"

#include "physx_diagnostics.h"

namespace physx
{
    class PxDefaultAllocator;
    class PxDefaultCpuDispatcher;
    class PxFoundation;
    class PxPvd;
    class PxPhysics;
    class PxCooking;
}

namespace smile::physics
{
    class PhysXPhysicsEngine final : public PhysicsEngine
    {
      public:
        PhysXPhysicsEngine();
        ~PhysXPhysicsEngine();

        void *GetPhysics() const override
        {
            return m_pPhysics;
        }

        physx::PxDefaultCpuDispatcher* GetDefaultCpuDispatcher() const
        {
            return m_pDefaultCpuDispatcher;
        }

      private:
        physx::PxDefaultAllocator m_AllocatorCallback;
        physx::PxDefaultCpuDispatcher *m_pDefaultCpuDispatcher{};
        physx::PxFoundation *m_pFoundation{};
        physx::PxPvd *m_pPvd{};
        physx::PxPhysics *m_pPhysics{};
        physx::PxCooking *m_pCookingFactory{};

        PhysXErrorCallback m_ErrorCallback;
        PhysXAssertHandler m_AssertHandler{};
    };
}