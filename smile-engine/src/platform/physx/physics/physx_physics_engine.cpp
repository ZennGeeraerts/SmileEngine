/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physx_physics_engine.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    PhysXPhysicsEngine::PhysXPhysicsEngine()
    {
        // Setup the foundation
        m_pFoundation = PxCreateFoundation( PX_PHYSICS_VERSION, m_AllocatorCallback, m_ErrorCallback );
        SM_ASSERT( m_pFoundation, "PhysXPhysicsEngine::PhysXPhysicsEngine > Failed to create PhysX foundation" );

        // Create a PDV instance
        m_pPvd = PxCreatePvd( *m_pFoundation );
        if ( m_pPvd )
        {
            physx::PxPvdTransport *pTransport = physx::PxDefaultPvdSocketTransportCreate( "localhost", 0001, 10 );
            m_pPvd->connect( *pTransport, physx::PxPvdInstrumentationFlag::eALL );
        }

        // Create an instance of the PhysX physics SDK
        physx::PxTolerancesScale scale = physx::PxTolerancesScale();
        scale.length = 10;
        m_pPhysics = PxCreatePhysics( PX_PHYSICS_VERSION, *m_pFoundation, scale, true, m_pPvd );
        SM_ASSERT( m_pPhysics, "PhysXPhysicsEngine::PhysXPhysicsEngine > Failed to create PhysX physics" );

        // Create the cooking factory
        m_pCookingFactory = PxCreateCooking( PX_PHYSICS_VERSION, *m_pFoundation, m_pPhysics->getTolerancesScale() );
        SM_ASSERT( m_pCookingFactory, "PhysXPhysicsEngine::PhysXPhysicsEngine > Failed to create PhysX cooking" );
        m_pDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
        PxSetAssertHandler( m_AssertHandler );

        SM_LOG_INFO( "Initialized PhysX Physics Engine" );
    }

    PhysXPhysicsEngine::~PhysXPhysicsEngine()
    {
        if ( m_pCookingFactory )
        {
            m_pCookingFactory->release();
            m_pCookingFactory = nullptr;
        }
        if ( m_pPhysics )
        {
            m_pPhysics->release();
            m_pPhysics = nullptr;
        }
        if ( m_pDefaultCpuDispatcher )
        {
            m_pDefaultCpuDispatcher->release();
            m_pDefaultCpuDispatcher = nullptr;
        }
        if ( m_pFoundation )
        {
            m_pFoundation->release();
            m_pFoundation = nullptr;
        }
    }
}