/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "engine/physics/physics_engine.h"

#include "physx_diagnostics.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    struct PhysicsEngine::Opaque final
    {
        std::vector< Ref< PhysicsWorld > > pWorlds{};
        std::vector< Ref< PhysicsMaterial > > pMaterials{};

        PhysicsEngineData PhysicsEngineData{};

        physx::PxDefaultAllocator AllocatorCallback;
        physx::PxDefaultCpuDispatcher *pDefaultCpuDispatcher{};
        physx::PxFoundation *pFoundation{};
        physx::PxPvd *pPvd{};
        physx::PxPhysics *pPhysics{};
        physx::PxCooking *pCookingFactory{};

        PhysXErrorCallback ErrorCallback;
        PhysXAssertHandler AssertHandler{};
    };

    PhysicsEngine::PhysicsEngine()
    {
        // Setup the foundation
        m_pImplementation->pFoundation = PxCreateFoundation(
            PX_PHYSICS_VERSION, m_pImplementation->AllocatorCallback, m_pImplementation->ErrorCallback );
        SM_ASSERT( m_pImplementation->pFoundation,
            "PhysXPhysicsEngine::PhysXPhysicsEngine > Failed to create PhysX foundation" );

        // Create a PDV instance
        m_pImplementation->pPvd = PxCreatePvd( *m_pImplementation->pFoundation );
        if ( m_pImplementation->pPvd )
        {
            physx::PxPvdTransport *pTransport = physx::PxDefaultPvdSocketTransportCreate( "localhost", 0001, 10 );
            m_pImplementation->pPvd->connect( *pTransport, physx::PxPvdInstrumentationFlag::eALL );
        }

        // Create an instance of the PhysX physics SDK
        physx::PxTolerancesScale scale = physx::PxTolerancesScale();
        scale.length = 10;
        m_pImplementation->pPhysics = PxCreatePhysics(
            PX_PHYSICS_VERSION, *m_pImplementation->pFoundation, scale, true, m_pImplementation->pPvd );
        SM_ASSERT( m_pImplementation->pPhysics, "PhysicsEngine::PhysicsEngine > Failed to create PhysX physics" );

        // Create the cooking factory
        m_pImplementation->pCookingFactory = PxCreateCooking(
            PX_PHYSICS_VERSION, *m_pImplementation->pFoundation, m_pImplementation->pPhysics->getTolerancesScale() );
        SM_ASSERT( m_pImplementation->pCookingFactory,
            "PhysXPhysicsEngine::PhysXPhysicsEngine > Failed to create PhysX cooking" );
        m_pImplementation->pDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
        PxSetAssertHandler( m_pImplementation->AssertHandler );

        SM_LOG_INFO( "Initialized PhysX Physics Engine" );
    }

    PhysicsEngine::~PhysicsEngine()
    {
        if ( m_pImplementation->pCookingFactory )
        {
            m_pImplementation->pCookingFactory->release();
            m_pImplementation->pCookingFactory = nullptr;
        }
        if ( m_pImplementation->pPhysics )
        {
            m_pImplementation->pPhysics->release();
            m_pImplementation->pPhysics = nullptr;
        }
        if ( m_pImplementation->pDefaultCpuDispatcher )
        {
            m_pImplementation->pDefaultCpuDispatcher->release();
            m_pImplementation->pDefaultCpuDispatcher = nullptr;
        }
        if ( m_pImplementation->pFoundation )
        {
            m_pImplementation->pFoundation->release();
            m_pImplementation->pFoundation = nullptr;
        }
    }

    Ref< PhysicsWorld > PhysicsEngine::CreateWorld( const PhysicsWorldSettings &worldSettings )
    {
        auto pWorld = CreateRef< PhysicsWorld >( this, worldSettings );
        m_pImplementation->pWorlds.emplace_back( pWorld );
        return pWorld;
    }

    void PhysicsEngine::DestroyWorld( Ref< PhysicsWorld > pPhysicsWorld )
    {
        m_pImplementation->pWorlds.erase(
            std::remove( m_pImplementation->pWorlds.begin(), m_pImplementation->pWorlds.end(), pPhysicsWorld ) );
    }

    Ref< PhysicsMaterial >
    PhysicsEngine::CreateMaterial( float staticFriction, float dynamicFriction, float restitution )
    {
        auto pMaterial = CreateRef< PhysicsMaterial >( staticFriction, dynamicFriction, restitution );
        m_pImplementation->pMaterials.emplace_back( pMaterial );
        return pMaterial;
    }

    void PhysicsEngine::DestroyMaterial( Ref< PhysicsMaterial > pPhysicsMaterial )
    {
        m_pImplementation->pMaterials.erase( std::remove(
            m_pImplementation->pMaterials.begin(), m_pImplementation->pMaterials.end(), pPhysicsMaterial ) );
    }

    bool PhysicsEngine::OnSimulate( primitive::Timestep deltaTime )
    {
        return Advance( deltaTime );
    }

    bool PhysicsEngine::Advance( primitive::Timestep deltaTime )
    {
        SubstepStrategy( deltaTime );

        if ( m_pImplementation->PhysicsEngineData.SubstepCount == 0 )
            return false;

        for ( Uint32 i{}; i < m_pImplementation->PhysicsEngineData.SubstepCount; ++i )
        {
            for ( auto pWorld : m_pImplementation->pWorlds )
                pWorld->OnSimulate( m_pImplementation->PhysicsEngineData.FixedTimestep );
        }

        return true;
    }

    void PhysicsEngine::SubstepStrategy( primitive::Timestep deltaTime )
    {
        if ( m_pImplementation->PhysicsEngineData.Accumulator > m_pImplementation->PhysicsEngineData.FixedTimestep )
            m_pImplementation->PhysicsEngineData.Accumulator = 0.0f;

        m_pImplementation->PhysicsEngineData.Accumulator += deltaTime;
        if ( m_pImplementation->PhysicsEngineData.Accumulator < m_pImplementation->PhysicsEngineData.FixedTimestep )
        {
            m_pImplementation->PhysicsEngineData.SubstepCount = 0;
            return;
        }

        m_pImplementation->PhysicsEngineData.SubstepCount =
            std::min( static_cast< Uint32 >( m_pImplementation->PhysicsEngineData.Accumulator /
                                             m_pImplementation->PhysicsEngineData.FixedTimestep ),
                m_pImplementation->PhysicsEngineData.MaxSubsteps );

        m_pImplementation->PhysicsEngineData.Accumulator -=
            m_pImplementation->PhysicsEngineData.SubstepCount * m_pImplementation->PhysicsEngineData.FixedTimestep;
    }

    void *PhysicsEngine::GetPhysics() const
    {
        return m_pImplementation->pPhysics;
    }

    void *PhysicsEngine::GetDefaultCpuDispatcher() const
    {
        return m_pImplementation->pDefaultCpuDispatcher;
    }
}