#include "smpch.h"
#include "physics_engine.h"

#include "smile_engine/core/logger.h"
#include "physics_utils.h"
#include "physics_diagnostics.h"
#include "contact_listener.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    std::unordered_map< UUID, Ref< PhysicsActor > > PhysicsEngine::actorMap{};
    PhysicsSettings PhysicsEngine::settings{};
    PhysicsEngineData PhysicsEngine::physicsEngineData{};

    static physx::PxScene *scene;

    static physx::PxDefaultAllocator allocatorCallback;
    static physx::PxDefaultCpuDispatcher *defaultCpuDispatcher{};
    static physx::PxFoundation *foundation{};
    static physx::PxPvd *pvd{};
    static physx::PxPhysics *physics{};
    static physx::PxCooking *cookingFactory{};

    static PhysicsErrorCallback errorCallback;
    static PhysicsAssertHandler assertHandler{};
    static ContactListener contactListener{};

    void PhysicsEngine::initialize()
    {
        SM_ASSERT( !foundation, "PhysicsEngine::initialize > Physics Engine is already initialized" );

        // Setup the foundation
        foundation = PxCreateFoundation( PX_PHYSICS_VERSION, allocatorCallback, errorCallback );
        SM_ASSERT( foundation, "PhysicsEngine::initialize > Failed to create PhysX foundation" );

        // Create a PDV instance
        pvd = PxCreatePvd( *foundation );
        if ( pvd )
        {
            physx::PxPvdTransport *transport = physx::PxDefaultPvdSocketTransportCreate( "localhost", 0001, 10 );
            pvd->connect( *transport, physx::PxPvdInstrumentationFlag::eALL );
        }

        // Create an instance of the PhysX physics SDK
        physx::PxTolerancesScale scale = physx::PxTolerancesScale();
        scale.length = 10;
        physics = PxCreatePhysics( PX_PHYSICS_VERSION, *foundation, scale, true, pvd );
        SM_ASSERT( physics, "PhysicsEngine::initialize > Failed to create PhysX Physics" );

        // Create the cooking factory
        cookingFactory = PxCreateCooking( PX_PHYSICS_VERSION, *foundation, physics->getTolerancesScale() );
        SM_ASSERT( cookingFactory, "PhysicsEngine::initialize > Failed to create PhysX Cooking" );
        defaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
        PxSetAssertHandler( assertHandler );

        // Create default physics material
        settings.defaultPhysicsMaterial = createRef< PhysicsMaterial >();
        settings.defaultPhysicsMaterial->staticFriction = 0.3f;
        settings.defaultPhysicsMaterial->dynamicFriction = 0.3f;
        settings.defaultPhysicsMaterial->restitution = 0.4f;

        SM_LOG_INFO( "Initialized Physics Engine" );
    }

    void PhysicsEngine::shutDown()
    {
        if ( scene )
            destroyScene();

        if ( cookingFactory )
        {
            cookingFactory->release();
            cookingFactory = nullptr;
        }
        if ( physics )
        {
            physics->release();
            physics = nullptr;
        }
        if ( defaultCpuDispatcher )
        {
            defaultCpuDispatcher->release();
            defaultCpuDispatcher = nullptr;
        }
        if ( foundation )
        {
            foundation->release();
            foundation = nullptr;
        }
    }

    static physx::PxBroadPhaseType::Enum smileToPhysXBroadPhaseType( BroadPhaseType type )
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

    static physx::PxFrictionType::Enum smileToPhysXFrictionType( FrictionType type )
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

    void PhysicsEngine::createScene()
    {
        SM_ASSERT( !scene, "PhysicsEngine::createScene > Scene already has a physics scene" );

        physx::PxSceneDesc scene_desc{ physics->getTolerancesScale() };
        scene_desc.gravity = utils::convertToPhysXVector( settings.gravity );
        scene_desc.filterShader = utils::smileSimulationFilterShader;
        scene_desc.cpuDispatcher = defaultCpuDispatcher;
        scene_desc.simulationEventCallback = &contactListener;
        scene_desc.broadPhaseType = smileToPhysXBroadPhaseType( settings.broadPhaseAlgorithm );
        scene_desc.frictionType = smileToPhysXFrictionType( settings.frictionModel );
        scene_desc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection

        SM_ASSERT( scene_desc.isValid(), "PhysicsEngine::createScene > Scene descriptor is not valid" );
        scene = physics->createScene( scene_desc );

        if ( settings.broadPhaseAlgorithm != BroadPhaseType::AutomaticBoxPrune )
        {
            physx::PxBounds3 *region_bounds = nullptr;
            physx::PxBounds3 global_bounds{ utils::convertToPhysXVector( settings.worldBoundsMin ),
                utils::convertToPhysXVector( settings.worldBoundsMax ) };
            Uint32 region_count = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(
                region_bounds, global_bounds, settings.worldBoundsSubdivisions );

            for ( Uint32 i{}; i < region_count; ++i )
            {
                physx::PxBroadPhaseRegion region{};
                region.bounds = region_bounds[i];
                scene->addBroadPhaseRegion( region );
            }
        }
    }

    void PhysicsEngine::destroyScene()
    {
        SM_ASSERT( scene, "PhysicsEngine::destroyScene > Scene is not valid" );

        for ( auto &actor : actorMap )
            actor.second.reset();

        actorMap.clear();

        scene->release();
        scene = nullptr;
    }

    Ref< PhysicsActor > PhysicsEngine::createActor( scene::Entity entity )
    {
        SM_ASSERT( scene, "PhysicsEngine::createActor > Scene is not valid" );

        Ref< PhysicsActor > actor = createRef< PhysicsActor >( entity );
        actorMap[entity.getUUID()] = actor;
        scene->addActor( *actor->rigidActor );
        return actor;
    }

    Ref< PhysicsActor > PhysicsEngine::getActorOfEntity( scene::Entity entity )
    {
        auto it = actorMap.find( entity.getUUID() );
        if ( it != actorMap.end() )
            return ( *it ).second;

        return nullptr;
    }

    void PhysicsEngine::simulate( Timestep delta_time )
    {
        if ( advance( delta_time ) )
        {
            for ( const auto &actor : actorMap )
                actor.second->updateTransform();
        }
    }

    bool PhysicsEngine::advance( Timestep delta_time )
    {
        substepStrategy( delta_time );

        if ( physicsEngineData.substepCount == 0 )
            return false;

        for ( Uint32 i{}; i < physicsEngineData.substepCount; ++i )
        {
            scene->simulate( settings.fixedTimestep );
            scene->fetchResults( true );
        }

        return true;
    }

    void PhysicsEngine::substepStrategy( Timestep delta_time )
    {
        if ( physicsEngineData.accumulator > settings.fixedTimestep )
            physicsEngineData.accumulator = 0.0f;

        physicsEngineData.accumulator += delta_time;
        if ( physicsEngineData.accumulator < settings.fixedTimestep )
        {
            physicsEngineData.substepCount = 0;
            return;
        }

        physicsEngineData.substepCount =
            std::min( static_cast< Uint32 >( physicsEngineData.accumulator / settings.fixedTimestep ),
                settings.maxSubsteps );
        physicsEngineData.accumulator -= physicsEngineData.substepCount * settings.fixedTimestep;
    }

    physx::PxPhysics *PhysicsEngine::getPhysics()
    {
        return physics;
    }

    physx::PxAllocatorCallback &PhysicsEngine::getAllocatorCallback()
    {
        return allocatorCallback;
    }
}