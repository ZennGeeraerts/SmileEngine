#include "smpch.h"
#include "physics_engine.h"

#include "smile_engine/core/logger.h"
#include "physics_utils.h"
#include "physics_diagnostics.h"
#include "contact_listener.h"

#include <PxPhysicsAPI.h>

namespace smile
{
    std::unordered_map< UUID, Ref< PhysicsActor > > PhysicsEngine::s_ActorMap{};
    PhysicsSettings PhysicsEngine::s_Settings{};
    PhysicsEngineData PhysicsEngine::s_PhysicsEngineData{};

    static physx::PxScene *s_pScene;

    static physx::PxDefaultAllocator s_AllocatorCallback;
    static physx::PxDefaultCpuDispatcher *s_pDefaultCpuDispatcher{};
    static physx::PxFoundation *s_pFoundation{};
    static physx::PxPvd *s_pPVD{};
    static physx::PxPhysics *s_pPhysics{};
    static physx::PxCooking *s_pCookingFactory{};

    static PhysicsErrorCallback s_ErrorCallback;
    static PhysicsAssertHandler s_AssertHandler{};
    static ContactListener s_ContactListener{};

    void PhysicsEngine::Initialize()
    {
        SM_ASSERT( !s_pFoundation, "PhysicsEngine::Init > Physics Engine is already initialized" );

        // Setup the foundation
        s_pFoundation = PxCreateFoundation( PX_PHYSICS_VERSION, s_AllocatorCallback, s_ErrorCallback );
        SM_ASSERT( s_pFoundation, "PhysicsEngine::Init > Failed to create PhysX foundation" );

        // Create a PDV instance
        s_pPVD = PxCreatePvd( *s_pFoundation );
        if ( s_pPVD )
        {
            physx::PxPvdTransport *pTransport = physx::PxDefaultPvdSocketTransportCreate( "localhost", 0001, 10 );
            s_pPVD->connect( *pTransport, physx::PxPvdInstrumentationFlag::eALL );
        }

        // Create an instance of the PhysX physics SDK
        physx::PxTolerancesScale scale = physx::PxTolerancesScale();
        scale.length = 10;
        s_pPhysics = PxCreatePhysics( PX_PHYSICS_VERSION, *s_pFoundation, scale, true, s_pPVD );
        SM_ASSERT( s_pPhysics, "PhysicsEngine::Init > Failed to create PhysX Physics" );

        // Create the cooking factory
        s_pCookingFactory = PxCreateCooking( PX_PHYSICS_VERSION, *s_pFoundation, s_pPhysics->getTolerancesScale() );
        SM_ASSERT( s_pCookingFactory, "PhysicsEngine::Init > Failed to create PhysX Cooking" );
        s_pDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
        PxSetAssertHandler( s_AssertHandler );

        // Create default physics material
        s_Settings.m_pDefaultPhysicsMaterial = CreateRef< PhysicsMaterial >();
        s_Settings.m_pDefaultPhysicsMaterial->m_StaticFriction = 0.3f;
        s_Settings.m_pDefaultPhysicsMaterial->m_DynamicFriction = 0.3f;
        s_Settings.m_pDefaultPhysicsMaterial->m_Restitution = 0.4f;

        SM_LOG_INFO( "Initialized Physics Engine" );
    }

    void PhysicsEngine::ShutDown()
    {
        if ( s_pCookingFactory )
        {
            s_pCookingFactory->release();
            s_pCookingFactory = nullptr;
        }
        if ( s_pPhysics )
        {
            s_pPhysics->release();
            s_pPhysics = nullptr;
        }
        if ( s_pDefaultCpuDispatcher )
        {
            s_pDefaultCpuDispatcher->release();
            s_pDefaultCpuDispatcher = nullptr;
        }
        if ( s_pFoundation )
        {
            s_pFoundation->release();
            s_pFoundation = nullptr;
        }
    }

    static physx::PxBroadPhaseType::Enum SmileToPhysXBroadPhaseType( BroadPhaseType type )
    {
        switch ( type )
        {
            case BroadPhaseType::SweepAndPrune:
                return physx::PxBroadPhaseType::eSAP;
            case BroadPhaseType::MultiBoxPrune:
                return physx::PxBroadPhaseType::eMBP;
            case BroadPhaseType::AutomaticBoxPrune:
                return physx::PxBroadPhaseType::eABP;
            default:
                return physx::PxBroadPhaseType::eABP;
        }
    }

    static physx::PxFrictionType::Enum SmileToPhysXFrictionType( FrictionType type )
    {
        switch ( type )
        {
            case FrictionType::Patch:
                return physx::PxFrictionType::ePATCH;
            case FrictionType::OneDirectional:
                return physx::PxFrictionType::eONE_DIRECTIONAL;
            case FrictionType::TwoDirectional:
                return physx::PxFrictionType::eTWO_DIRECTIONAL;
            default:
                return physx::PxFrictionType::ePATCH;
        }
    }

    void PhysicsEngine::CreateScene()
    {
        SM_ASSERT( !s_pScene, "PhysicsEngine::CreateScene > Scene already has a physics scene" );

        physx::PxSceneDesc sceneDesc{ s_pPhysics->getTolerancesScale() };
        sceneDesc.gravity = utils::ConvertToPhysXVector( s_Settings.m_Gravity );
        sceneDesc.filterShader = utils::SmileSimulationFilterShader;
        sceneDesc.cpuDispatcher = s_pDefaultCpuDispatcher;
        sceneDesc.simulationEventCallback = &s_ContactListener;
        sceneDesc.broadPhaseType = SmileToPhysXBroadPhaseType( s_Settings.m_BroadPhaseAlgorithm );
        sceneDesc.frictionType = SmileToPhysXFrictionType( s_Settings.m_FrictionModel );
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection

        SM_ASSERT( sceneDesc.isValid(), "PhysicsEngine::CreateScene > Scene descriptor is not valid" );
        s_pScene = s_pPhysics->createScene( sceneDesc );

        if ( s_Settings.m_BroadPhaseAlgorithm != BroadPhaseType::AutomaticBoxPrune )
        {
            physx::PxBounds3 *pRegionBounds = nullptr;
            physx::PxBounds3 globalBounds{ utils::ConvertToPhysXVector( s_Settings.m_WorldBoundsMin ),
                utils::ConvertToPhysXVector( s_Settings.m_WorldBoundsMax ) };
            uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(
                pRegionBounds, globalBounds, s_Settings.m_WorldBoundsSubdivisions );

            for ( uint32_t i{}; i < regionCount; ++i )
            {
                physx::PxBroadPhaseRegion region{};
                region.bounds = pRegionBounds[i];
                s_pScene->addBroadPhaseRegion( region );
            }
        }
    }

    void PhysicsEngine::DestroyScene()
    {
        SM_ASSERT( s_pScene, "PhysicsEngine::DestroyScene > Scene is not valid" );

        for ( auto &actor : s_ActorMap )
            actor.second.reset();

        s_ActorMap.clear();

        s_pScene->release();
        s_pScene = nullptr;
    }

    Ref< PhysicsActor > PhysicsEngine::CreateActor( Entity entity )
    {
        SM_ASSERT( s_pScene, "PhysicsEngine::CreateActor > Scene is not valid" );

        Ref< PhysicsActor > pActor = CreateRef< PhysicsActor >( entity );
        s_ActorMap[entity.GetUUID()] = pActor;
        s_pScene->addActor( *pActor->m_pRigidActor );
        return pActor;
    }

    Ref< PhysicsActor > PhysicsEngine::GetActorOfEntity( Entity entity )
    {
        auto it = s_ActorMap.find( entity.GetUUID() );
        if ( it != s_ActorMap.end() )
            return ( *it ).second;

        return nullptr;
    }

    void PhysicsEngine::Simulate( Timestep deltaTime )
    {
        if ( Advance( deltaTime ) )
        {
            for ( const auto &actor : s_ActorMap )
                actor.second->UpdateTransform();
        }
    }

    bool PhysicsEngine::Advance( Timestep deltaTime )
    {
        SubstepStrategy( deltaTime );

        if ( s_PhysicsEngineData.m_SubstepCount == 0 )
            return false;

        for ( uint32_t i{}; i < s_PhysicsEngineData.m_SubstepCount; ++i )
        {
            s_pScene->simulate( s_Settings.m_FixedTimestep );
            s_pScene->fetchResults( true );
        }

        return true;
    }

    void PhysicsEngine::SubstepStrategy( Timestep deltaTime )
    {
        if ( s_PhysicsEngineData.m_Accumulator > s_Settings.m_FixedTimestep )
            s_PhysicsEngineData.m_Accumulator = 0.0f;

        s_PhysicsEngineData.m_Accumulator += deltaTime;
        if ( s_PhysicsEngineData.m_Accumulator < s_Settings.m_FixedTimestep )
        {
            s_PhysicsEngineData.m_SubstepCount = 0;
            return;
        }

        s_PhysicsEngineData.m_SubstepCount =
            std::min( static_cast< uint32_t >( s_PhysicsEngineData.m_Accumulator / s_Settings.m_FixedTimestep ),
                s_Settings.m_MaxSubsteps );
        s_PhysicsEngineData.m_Accumulator -= s_PhysicsEngineData.m_SubstepCount * s_Settings.m_FixedTimestep;
    }

    physx::PxPhysics *PhysicsEngine::GetPhysics()
    {
        return s_pPhysics;
    }

    physx::PxAllocatorCallback &PhysicsEngine::GetAllocatorCallback()
    {
        return s_AllocatorCallback;
    }
}