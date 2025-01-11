/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "engine/physics/physics_world.h"

#include "engine/physics/physics_engine.h"
#include "engine/physics/rigidbody.h"

#include "physx_utils.h"
#include "contact_listener.h"

#include "engine/graphic/renderer/debug_renderer.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    struct PhysicsWorld::Opaque
    {
        const PhysicsEngine *pPhysicsEngine;
        PhysicsWorldSettings Settings;
        std::vector< Ref< Rigidbody > > pRigidbodies;
        std::vector< Ref< CharacterController > > pCharacterControllers;

        ContactListener ContactListener{};
        physx::PxScene *pScene;
        physx::PxControllerManager *pControllerManager;
    };

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

    PhysicsWorld::PhysicsWorld( const PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings )
    {
        m_pImplementation->pPhysicsEngine = pPhysicsEngine;
        m_pImplementation->Settings = settings;

        auto pPhysics = static_cast< physx::PxPhysics * >( m_pImplementation->pPhysicsEngine->GetPhysics() );

        physx::PxSceneDesc sceneDesc{ pPhysics->getTolerancesScale() };
        sceneDesc.gravity = utils::ConvertToPhysXVector( m_pImplementation->Settings.Gravity );
        sceneDesc.filterShader = utils::SmileSimulationFilterShader;
        sceneDesc.cpuDispatcher =
            static_cast< physx::PxCpuDispatcher * >( m_pImplementation->pPhysicsEngine->GetDefaultCpuDispatcher() );
        sceneDesc.simulationEventCallback = &m_pImplementation->ContactListener;
        sceneDesc.broadPhaseType = SmileToPhysXBroadPhaseType( m_pImplementation->Settings.BroadPhaseAlgorithm );
        sceneDesc.frictionType = SmileToPhysXFrictionType( m_pImplementation->Settings.FrictionModel );
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection

        SM_ASSERT( sceneDesc.isValid(), "PhysicsWorld::PhysicsWorld > Scene descriptor is not valid" );
        m_pImplementation->pScene = pPhysics->createScene( sceneDesc );

        if ( m_pImplementation->Settings.BroadPhaseAlgorithm != BroadPhaseType::AutomaticBoxPrune )
        {
            physx::PxBounds3 *pRegionBounds = nullptr;
            physx::PxBounds3 globalBounds{ utils::ConvertToPhysXVector( m_pImplementation->Settings.WorldBoundsMin ),
                utils::ConvertToPhysXVector( m_pImplementation->Settings.WorldBoundsMax ) };
            Uint32 regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(
                pRegionBounds, globalBounds, m_pImplementation->Settings.WorldBoundsSubdivisions );

            for ( Uint32 i{}; i < regionCount; ++i )
            {
                physx::PxBroadPhaseRegion region{};
                region.bounds = pRegionBounds[i];
                m_pImplementation->pScene->addBroadPhaseRegion( region );
            }
        }

        m_pImplementation->pScene->setVisualizationParameter( physx::PxVisualizationParameter::eSCALE, 1.0f );
        m_pImplementation->pScene->setVisualizationParameter(
            physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f );
        m_pImplementation->pScene->setVisualizationParameter( physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f );
        m_pImplementation->pScene->setVisualizationParameter(
            physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f );

        m_pImplementation->pControllerManager = PxCreateControllerManager( *m_pImplementation->pScene );
        SM_ASSERT( m_pImplementation->pControllerManager, "PhysicsWorld > Failed to create controller manager" );
    }

    PhysicsWorld::~PhysicsWorld()
    {
        m_pImplementation->pRigidbodies.clear();
        m_pImplementation->pCharacterControllers.clear();

        if ( m_pImplementation->pControllerManager )
        {
            m_pImplementation->pControllerManager->release();
            m_pImplementation->pControllerManager = nullptr;
        }

        if ( m_pImplementation->pScene )
        {
            m_pImplementation->pScene->release();
            m_pImplementation->pScene = nullptr;
        }
    }

    Ref< Rigidbody > PhysicsWorld::CreateRigidbody( RigidbodyType bodyType,
        const DirectX::XMFLOAT4X4 &initialTransform )
    {
        auto pRigidbody = CreateRef< Rigidbody >( this, bodyType, initialTransform );
        m_pImplementation->pScene->addActor( *static_cast< physx::PxActor * >( pRigidbody->GetInternal() ) );
        m_pImplementation->pRigidbodies.emplace_back( pRigidbody );
        return pRigidbody;
    }

    void PhysicsWorld::DestroyRigidbody( Ref< Rigidbody > pRigidbody )
    {
        m_pImplementation->pRigidbodies.erase(
            std::remove( m_pImplementation->pRigidbodies.begin(), m_pImplementation->pRigidbodies.end(), pRigidbody ) );
    }

    Ref< CharacterController > PhysicsWorld::CreateCharacterController( float radius,
        float height,
        CharacterController::ClimbingModeType climbingMode,
        const DirectX::XMFLOAT3 &initialTranslation )
    {
        auto pCharacterController =
            CreateRef< CharacterController >( this, radius, height, climbingMode, initialTranslation );
        m_pImplementation->pCharacterControllers.emplace_back( pCharacterController );
        return pCharacterController;
    }

    void PhysicsWorld::DestroyCharacterController( Ref< CharacterController > pCharacterController )
    {
        m_pImplementation->pCharacterControllers.erase( std::remove( m_pImplementation->pCharacterControllers.begin(),
            m_pImplementation->pCharacterControllers.end(),
            pCharacterController ) );
    }

    void PhysicsWorld::OnSimulate( primitive::Timestep fixedDeltaTime )
    {
        m_pImplementation->pScene->simulate( fixedDeltaTime );
        m_pImplementation->pScene->fetchResults( true );
    }

    void PhysicsWorld::OnDebugRender()
    {
        if ( !m_pImplementation->pScene )
            return;

        const auto pRenderBuffer = &m_pImplementation->pScene->getRenderBuffer();
        const auto pDebugLines = pRenderBuffer->getLines();

        for ( Uint32 i{}; i < pRenderBuffer->getNbLines(); ++i )
        {
            const auto &line = pDebugLines[i];

            const DirectX::XMFLOAT3 start = utils::ConvertToDirectXVector( line.pos0 );
            const DirectX::XMFLOAT3 end = utils::ConvertToDirectXVector( line.pos1 );
            const auto colorStart = utils::ConvertToDirectXColor( line.color0 );
            const auto colorEnd = utils::ConvertToDirectXColor( line.color1 );

            graphic::DebugRenderer::GetInstance().SubmitLine( start, end, colorStart, colorEnd );
        }
    }

    void *PhysicsWorld::GetInternal() const
    {
        return m_pImplementation->pScene;
    }

    void *PhysicsWorld::GetControllerManager() const
    {
        return m_pImplementation->pControllerManager;
    }

    const PhysicsWorldSettings &PhysicsWorld::GetSettings() const
    {
        return m_pImplementation->Settings;
    }
}