/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physx_physics_world.h"

#include "physx_utils.h"
#include "physx_physics_engine.h"
#include "contact_listener.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    static ContactListener s_ContactListener{};

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

    PhysXPhysicsWorld::PhysXPhysicsWorld( PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings )
        : PhysicsWorld{ pPhysicsEngine, settings }
    {
        auto pPhysics = static_cast< physx::PxPhysics * >( pPhysicsEngine->GetPhysics() );
        const auto pPhysXPhysicsEngine = static_cast< const PhysXPhysicsEngine * >( pPhysicsEngine );

        physx::PxSceneDesc sceneDesc{ pPhysics->getTolerancesScale() };
        sceneDesc.gravity = utils::ConvertToPhysXVector( m_Settings.Gravity );
        sceneDesc.filterShader = utils::SmileSimulationFilterShader;
        sceneDesc.cpuDispatcher = pPhysXPhysicsEngine->GetDefaultCpuDispatcher();
        sceneDesc.simulationEventCallback = &s_ContactListener;
        sceneDesc.broadPhaseType = SmileToPhysXBroadPhaseType( m_Settings.BroadPhaseAlgorithm );
        sceneDesc.frictionType = SmileToPhysXFrictionType( m_Settings.FrictionModel );
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection

        SM_ASSERT( sceneDesc.isValid(), "PhysXPhysicsWorld::PhysXPhysicsWorld > Scene descriptor is not valid" );
        m_pScene = pPhysics->createScene( sceneDesc );

        if ( m_Settings.BroadPhaseAlgorithm != BroadPhaseType::AutomaticBoxPrune )
        {
            physx::PxBounds3 *pRegionBounds = nullptr;
            physx::PxBounds3 globalBounds{ utils::ConvertToPhysXVector( m_Settings.WorldBoundsMin ),
                utils::ConvertToPhysXVector( m_Settings.WorldBoundsMax ) };
            Uint32 regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(
                pRegionBounds, globalBounds, m_Settings.WorldBoundsSubdivisions );

            for ( Uint32 i{}; i < regionCount; ++i )
            {
                physx::PxBroadPhaseRegion region{};
                region.bounds = pRegionBounds[i];
                m_pScene->addBroadPhaseRegion( region );
            }
        }

        m_pScene->setVisualizationParameter( physx::PxVisualizationParameter::eSCALE, 1.0f );
        m_pScene->setVisualizationParameter( physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f );
        m_pScene->setVisualizationParameter( physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f );
        m_pScene->setVisualizationParameter( physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f );

        s_pControllerManager = PxCreateControllerManager( *m_pScene );
        SM_ASSERT( s_pControllerManager, "PhysicsEngine::CreateScene > Failed to create controller manager" );
    }
}