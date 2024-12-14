/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile_engine/physics/physics_material.h"

#include "smile_engine/physics/physics_engine.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    struct PhysicsMaterial::Opaque final
    {
        float StaticFriction;
        float DynamicFriction;
        float Restitution;
        physx::PxMaterial *pMaterial;
    };

    PhysicsMaterial::PhysicsMaterial( float staticFriction, float dynamicFriction, float restitution )
    {
        m_pImplementation->StaticFriction = staticFriction;
        m_pImplementation->DynamicFriction = dynamicFriction;
        m_pImplementation->Restitution = restitution;

        auto pPxPhysics = reinterpret_cast< physx::PxPhysics * >( PhysicsEngine::GetInstance().GetPhysics() );
        m_pImplementation->pMaterial = pPxPhysics->createMaterial(
            m_pImplementation->StaticFriction, m_pImplementation->DynamicFriction, m_pImplementation->Restitution );
    }

    PhysicsMaterial::~PhysicsMaterial()
    {
        if ( m_pImplementation->pMaterial )
        {
            m_pImplementation->pMaterial->release();
            m_pImplementation->pMaterial = nullptr;
        }
    }

    void *PhysicsMaterial::GetInternal() const
    {
        return m_pImplementation->pMaterial;
    }
}